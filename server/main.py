import socket
import threading
import numpy as np

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# socket.setblocking(False)
socket.bind(('192.168.1.72', 50000))
socket.listen()

conn, address = socket.accept()
#conn.setblocking(False)
conn_data, address_data = socket.accept()
#conn_data.setblocking(False)

length = 10
#whole_array = np.empty((0, length), dtype=np.int16)

while True:
    try:
        mode = input("Please choose the mode:\n")
        # Send the data to client
        conn.sendall(int(mode).to_bytes(2, 'little'))
        print("Package sended!")

        # Recieve the data
        data = conn.recv(1024)
        while not data:
            data = conn.recv(1024)
        # Parse
        if not data:
            print('No data!')
        #numbers = np.frombuffer(data, dtype=np.int16)
        print(data)

        #whole_array = np.append(whole_array, [data], axis=0)

    except (KeyboardInterrupt, SystemExit):
        conn.close()
        #np.savetxt("result.csv", whole_array, delimiter=",")
        break