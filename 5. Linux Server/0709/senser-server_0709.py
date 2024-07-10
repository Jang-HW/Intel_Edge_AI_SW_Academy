import serial
import time
import tkinter as tk

x = 0
root = tk.Tk()
root.title("test ui")
canvas = tk.Canvas(root, width=400, height=300, bg="black")
canvas.pack()

#serial setupa
ser = serial.Serial("/dev/ttyACM0", 115200)

def updateCanvas():
    global x
    value = 0
    
    if(ser.in_waiting > 0):
        data = ser.read() # byte read
        if data == b'\x02':
            timeout = time.time() + 0.02  # 2ms
            lostData = False
            while ser.in_waiting < 3:
                if time.time() > timeout:
                    lostData = True
                    break
            if lostData == False:
                data = ser.read(3)
                # check CRC
                    # data is binary list
                    # 2 is STX
                crc = (2 + data[0] + data[1]) % 256
                if (crc == data[2]):
                     # data parsing
                     mask = b'\x7f'
                     # why is b'\x7f'? data added with 0x80, when it sent
                     value = int.from_bytes(bytes([data[1] & mask[0]])) << 7
                     value += int.from_bytes(bytes([data[0] & mask[0]]))
                     print(value)
                     
    #canvas.delete("all")
    canvas.create_line(x, value/55, x+1, value/55, fill="green", width=10)

    x += 1
    if(x >= 399):
        x = 0

    canvas.after(100, updateCanvas)

updateCanvas()
root.mainloop()