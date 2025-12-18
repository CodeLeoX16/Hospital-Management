# Use Ubuntu as base image
FROM ubuntu:22.04

# Set working directory
WORKDIR /app

# Install required packages
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

# Copy source files
COPY server_linux.cpp .

# Copy header files if they exist (using wildcard pattern)
COPY *.h . 2>/dev/null || :

# Compile the C++ server
RUN g++ -o server server_linux.cpp -lpthread

# Expose the port the server runs on
EXPOSE 8080

# Run the server
CMD ["./server"]
