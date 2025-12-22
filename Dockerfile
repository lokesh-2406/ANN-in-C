# Stage 1: build C binary
FROM gcc:12 as builder
WORKDIR /build
COPY . /build
# build binary, use your Makefile
RUN make

# Stage 2: runtime image with Python
FROM python:3.11-slim
WORKDIR /app

# system deps for running (if any)
RUN apt-get update && apt-get install -y --no-install-recommends ca-certificates && rm -rf /var/lib/apt/lists/*

# Copy binary and model artifact into the image.
# Option A: If you commit testing_net into repo, it will be available in the built context.
# Option B (recommended): use CI to download model artifact into build context, then COPY it here.
COPY --from=builder /build/main /app/main
# Copy model dir if present
COPY testing_net /app/testing_net

# Copy python app
COPY app.py /app/app.py
COPY requirements.txt /app/requirements.txt
# Optional: static files
COPY web /app/web

RUN pip install --no-cache-dir -r requirements.txt

# Create non-root user
RUN groupadd -g 1000 appuser && useradd -r -u 1000 -g appuser appuser
RUN chown -R appuser:appuser /app
USER appuser

# Use gunicorn with 4 workers (adjust for your CPU)
ENV PORT=5000
EXPOSE 5000
CMD ["gunicorn", "app:app", "-b", "0.0.0.0:5000", "-w", "4", "--timeout", "60"]
