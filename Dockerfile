# ============================
# Stage 1 — Build C Binary
# ============================
FROM gcc:12 AS builder

WORKDIR /build

# Copy all source code
COPY . /build

# Build C project (Makefile already present)
RUN make

# ============================
# Stage 2 — Runtime Image
# ============================
FROM python:3.11-slim

WORKDIR /app

# Install minimal system deps
RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Copy binary + model artifacts from builder stage
COPY --from=builder /build/main /app/main
COPY --from=builder /build/testing_net /app/testing_net

# Copy frontend
COPY web /app/web

# Copy the Flask app
COPY app.py /app/app.py
COPY requirements.txt /app/requirements.txt

# Install Python deps
RUN pip install --no-cache-dir -r requirements.txt

# Use non-root user
RUN adduser --disabled-password --gecos "" appuser
USER appuser

# Expose port
EXPOSE 10000

# Start Gunicorn server
CMD ["gunicorn", "app:app", "-b", "0.0.0.0:10000", "-w", "2", "--timeout", "60"]
