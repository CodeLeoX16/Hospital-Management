FROM gcc:latest

WORKDIR /app

# Copy only the required source file
COPY server_linux.cpp .

# Compile the server
RUN g++ -o server server_linux.cpp -pthread

# Expose the port the server will run on
EXPOSE 8080

# Run the server
CMD ["./server"]
