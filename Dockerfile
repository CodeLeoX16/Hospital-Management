# Use an official GCC image as the base
FROM gcc:latest

# Set the working directory inside the container
WORKDIR /app

# Copy the source code into the container
COPY . .

# Compile the C++ program (server_linux.cpp)
RUN g++ -o server server_linux.cpp

# Expose the port the server will run on (if applicable)
EXPOSE 8080

# Command to run the server
CMD ["./server"]
