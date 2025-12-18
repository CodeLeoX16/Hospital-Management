FROM gcc:latest

WORKDIR /app

# Install dependencies including jsoncpp library
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    libjsoncpp-dev \
    && rm -rf /var/lib/apt/lists/*

# Copy all source files
COPY . .

# Compile the C++ server with jsoncpp
RUN g++ -o server server.cpp -ljsoncpp -lpthread -std=c++11

# Expose port (Render will provide PORT env variable)
EXPOSE 8080

# Run the server
CMD ["./server"]