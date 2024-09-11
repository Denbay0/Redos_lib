import socket

def send_request(command):
    HOST = '127.0.0.1'
    PORT = 8080

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        s.sendall(command.encode())
        data = s.recv(1024)

    print(f"Received: {data.decode()}")

send_request("12 + 5")
send_request("20 / 2")
send_request("10 * 6")