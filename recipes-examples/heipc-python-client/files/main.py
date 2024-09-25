#!/usr/bin/python3

import grpc
import heipc_cpp_server_grpc_client.greeter_pb2 as greeter_pb2
import heipc_cpp_server_grpc_client.greeter_pb2_grpc as greeter_pb2_grpc

def run():
    # Create a channel and a stub (client)
    with grpc.insecure_channel('localhost:50051') as channel:
        stub = greeter_pb2_grpc.GreeterStub(channel)
        
        # Create a HelloRequest message
        request = greeter_pb2.HelloRequest(name='World')
        
        # Call the SayHello method on the server and get a response
        response = stub.SayHello(request)
        
        print("Greeter client received: " + response.message)

if __name__ == "__main__":
    run()