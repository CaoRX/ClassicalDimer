import numpy as np 
import matplotlib.pyplot as plt 
import struct
import sys

def load_data(file_no):
	file_path = '../build/data/corrM' + str(file_no) + '.dat'
	data_file = open(file_path, mode = 'rb')
	file_contents = data_file.read()
	ret = struct.iter_unpack('d', file_contents)
	ret = [a[0] for a in ret]
	return np.array(ret)

def load_data_D(file_no):
	file_path = '../build/data/corrD' + str(file_no) + '.dat'
	data_file = open(file_path, mode = 'rb')
	file_contents = data_file.read()
	ret = struct.iter_unpack('d', file_contents)
	ret = [a[0] for a in ret]
	return np.reshape(np.array(ret), (4, -1))

def plot_data(data, label_content):
	n = len(data)
	x = np.arange(n)
	plt.plot(x[1:], data[1:], 'o-', label = label_content)

data_no = 1559550007
sys_args = sys.argv
if (len(sys_args) > 1):
	data_no = int(sys_args[1])
dataM = load_data(data_no)
dataD = load_data_D(data_no)
D_label = ['D1', 'D2', 'DA', 'DB']

plot_data(dataM, 'M(r)')
set_n = 4
for i in range(set_n):
	plot_data(dataD[i], D_label[i])


plt.legend()
plt.show()