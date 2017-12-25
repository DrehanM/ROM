import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import socket


UDP_IP = ""  # Set to empty IP to accept all packets heard on the specified port
UDP_Port = 9999 # Select a port that will only be used by the ESP8266 and nothing else to prevent random packets
intval = 5 # Set the interval between graph updates (seconds)

# Creates and binds the socket to the seclected Port and IP
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_Port))

# Plot and plot variable set up
fig = plt.figure()
style.use('fivethirtyeight')
ax1 = fig.add_subplot(1,1,1)
xs = []
ys = []

#Main loop function that will receive the UDP packets, convert them to plot values, and graph them
def animate(i):
  x = 0
  data, addr = sock.recvfrom(1024) # Assigns the received packets to data
  data = str(data)

  # The following block of code is to convert the packet from byte to string then to int
  
  #######
  t = ""
  y = ""
  j = 2
  while data[j] != ",":
    t += data[j]
    j+=1

  j+=1
  while j < len(data)-1:
    y += data[j]
    j+=1

  t = int(t) * intval
  y = int(y)
  #######

  xs.append(t)
  ys.append(y)
  ax1.clear()
  ax1.plot(xs,ys)

# This will loop the above function and update every 5 seconds
freq = intval * 1000
ani = animation.FuncAnimation(fig, animate, interval=freq) 
plt.show()




