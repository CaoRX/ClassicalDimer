import numpy as np 
import matplotlib.pyplot as plt 
import struct

def load_data(file_no):
	file_path = '../build/data/corrM' + str(file_no) + '.dat'
	data_file = open(file_path, mode = 'rb')
	file_contents = data_file.read()
	ret = struct.iter_unpack('d', file_contents)
	ret = [a[0] for a in ret]
	return np.array(ret)

def plot_data(data):
	n = len(data)
	x = np.arange(n)
	plt.plot(x[1:], data[1:], 'o-')
	plt.show()


data_no = 1559550007
data = load_data(data_no)
plot_data(data)


