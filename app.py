from flask import Flask, request, jsonify, send_from_directory
import tempfile
import subprocess
import re



import os
app = Flask(__name__, static_folder="web")
BASE_DIR = os.path.dirname(os.path.abspath(__file__))

BINARY = os.path.join(BASE_DIR, "main.exe")
MODEL_DIR = os.path.join(BASE_DIR, "testing_net")

print("Binary path:", BINARY)
print("Model path:", MODEL_DIR)

# # Path to your binary (built in the container)
# BINARY = "main.exe"
# # Default model directory (will be copied/present in image)
# DEFAULT_MODEL_DIR = "/app/testing_net"

@app.route("/")
def serve_home():
    return send_from_directory(app.static_folder, "index.html")

@app.route("/health", methods=["GET"])
def health():
    return jsonify({"status":"ok"})

@app.route("/predict", methods=["POST"])
def predict():
    # Expect a form file field named 'file' (csv)
    if 'file' not in request.files:
        return jsonify({"error": "no file part"}), 400

    f = request.files['file']
    if f.filename == '':
        return jsonify({"error": "empty filename"}), 400

    # Save to temp file
    with tempfile.NamedTemporaryFile(delete=False, suffix=".csv") as tmp:
        f.save(tmp.name)
        tmp_path = tmp.name

    try:
        # Call the binary
        cmd = [BINARY, "--predict", "--input", tmp_path, "--model", MODEL_DIR]
        proc = subprocess.run(cmd, capture_output=True, text=True, timeout=30)

        stdout = proc.stdout.strip()
        stderr = proc.stderr.strip()
        ret = proc.returncode

        if ret != 0:
            # Try to parse stderr as JSON fallback
            return jsonify({"error": "binary failed", "stderr": stderr}), 500

        # Binary prints JSON: {"prediction": X}
        try:
            import json
            match = re.search(r"\{.*\}$", stdout.strip(), flags=re.DOTALL)
            if not match:
                return jsonify({"error": "cannot-parse-output", "raw": stdout}), 500

            json_str = match.group(0)
            resp = json.loads(json_str)
            return jsonify(resp)
        except Exception as e:
            return jsonify({"error": "cannot-parse-output", "raw": stdout}), 500

    finally:
        try:
            os.unlink(tmp_path)
        except Exception:
            pass

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
