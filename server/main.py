import socket
import numpy as np

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket.bind(("192.168.1.72", 50000))
socket.listen()

conn, address = socket.accept()

length = 10
whole_array = np.empty((0,length), dtype=np.int16)

while True:
    try:
        # Generate data for the experiment
        # индекс сигнала(тип)
        # частоты
        # амплитуды
        # TODO
        # режим, стейт-машина на контроллере
        # парсер для разных режимов
        # 1. смена типа управления / настороек контроллера
        # 2. режим отработки таектории (траектория и ее параметры)
        # 3. режим изменения сетпоинта с клавиатуры (вверх вниз + задать числом)
        # 4. режим инициализации (сброса)
        # во всех режимах принимаем данные (подумать как, 2 программы или 2 сокета!?)
        data = np.arange(length,  dtype=np.float32)
        # Send the data to client
        conn.sendall(data)
        print("Package sended!")

        # Recieve the answer
        data = conn.recv(1024)
        while not data:
            data = conn.recv(1024)
        # Parse
        if not data:
            print('No data!')
        numbers = np.frombuffer(data, dtype=np.int16)
        print(numbers)

        whole_array = np.append(whole_array, [numbers], axis=0)
        
    except (KeyboardInterrupt, SystemExit):
        conn.close()
        np.savetxt("result.csv", whole_array, delimiter=",")
        break
