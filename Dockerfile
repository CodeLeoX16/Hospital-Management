# Multi-stage build for C++ Hospital Management Server
FROM gcc:latest as builder

# Set working directory
WORKDIR /app

# Install required dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

# Copy source files
COPY server_linux.cpp .
COPY *.h* . 2>/dev/null || true

# Compile the C++ server with optimization and error handling
# -std=c++17: Use C++17 standard
# -O2: Optimization level 2
# -Wall: Enable all warnings
# -Wextra: Enable extra warnings
# -pthread: Enable pthread support if needed
# -o server: Output executable name
RUN g++ -std=c++17 -O2 -Wall -Wextra -pthread \
    server_linux.cpp \
    -o server \
    || { echo "Compilation failed!"; exit 1; }

# Runtime stage - use smaller base image
FROM ubuntu:22.04

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libstdc++6 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user for security
RUN useradd -m -u 1000 -s /bin/bash appuser

# Set working directory
WORKDIR /app

# Copy compiled binary from builder stage
COPY --from=builder /app/server .

# Copy any required data files or configurations
COPY --chown=appuser:appuser . .

# Change ownership to non-root user
RUN chown -R appuser:appuser /app

# Switch to non-root user
USER appuser

# Expose port 8080
EXPOSE 8080

# Health check (optional but recommended)
HEALTHCHECK --interval=30s --timeout=3s --start-period=5s --retries=3 \
    CMD ["true"]

# Set environment variables
ENV PORT=8080
ENV SERVER_HOST=0.0.0.0

# Run the server with error handling
CMD ["./server"]

# Alternative CMD with error logging:
# CMD ["sh", "-c", "./server 2>&1 | tee /app/server.log"]
