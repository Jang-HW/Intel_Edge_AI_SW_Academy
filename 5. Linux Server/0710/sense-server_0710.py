import struct
import serial
import time
import tkinter as tk

def cmdStart():
    ser.write(bytes(b"\nSTART"))
    
def cmdStop():
    ser.write(bytes(b"\nSTOP"))

def updateCanvas():
    global x
    value = 0
    
    if(ser.in_waiting > 1):
        data = ser.read(2) # byte read
        # 2 bytes compare
        if data == b'\x11\x22':
            timeout = time.time() + 0.02  # 2ms
            lostData = False
            while ser.in_waiting < 2:
                if time.time() > timeout:
                    lostData = True
                    break
            if lostData == False:
                data = ser.read(2)
                # data parsing
                value = struct.unpack('<h', data)[0]
                print(value)           
    canvas.create_rectangle(0, 399, 299, 299-(value/55), fill="green")
    x += 1

    if(x >= 399):
        x = 0
        canvas.delete("all")
    canvas.after(100, updateCanvas)

x = 0
root = tk.Tk()
root.title("test ui")
canvas = tk.Canvas(root, width=400, height=300, bg="black")
canvas.grid(row=0, column=0, columnspan=2)

# button obj create
btnStart = tk.Button(text="Start", width=25, height=5, bg="gray", fg="white", command=cmdStart)
btnStart.grid(row=1, column=0)

btnStop = tk.Button(text="Stop", width=25, height=5, bg="gray", fg="white", command=cmdStop)
btnStop.grid(row=1, column=1)

#serial setupa
ser = serial.Serial("/dev/ttyACM0", 115200)
updateCanvas()
root.mainloop()
