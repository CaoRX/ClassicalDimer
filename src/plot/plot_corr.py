import numpy as np 
import matplotlib.pyplot as plt 
from load_binary_file import *
import sys

plot_M_flag = True
plot_D_flag = True
#def load_data(file_no):
#	file_path = '../build/data/corrM' + str(file_no) + '.dat'
#	data_file = open(file_path, mode = 'rb')
#	file_contents = data_file.read()
#	ret = struct.iter_unpack('d', file_contents)
#	ret = [a[0] for a in ret]
#	return np.array(ret)

#def load_data_D(file_no):
#	file_path = '../build/data/corrD' + str(file_no) + '.dat'
#	data_file = open(file_path, mode = 'rb')
#	file_contents = data_file.read()
#	ret = struct.iter_unpack('d', file_contents)
#	ret = [a[0] for a in ret]
#	return np.reshape(np.array(ret), (4, -1))

def plot_data(data, label_content, normalize_label = -1):
	n = len(data)
	x = np.arange(n)
	if (normalize_label != -1):
		data = data / data[normalize_label]
	plt.plot(x, data, 'o-', label = label_content)

def load_data_D(data_no):
	return load_file(get_filename('corrD', data_no), data_shape = (4, -1), data_type = 'double')
def load_data_M(data_no):
	return load_file(get_filename('corrM', data_no), data_shape = (1, -1), data_type = 'double')
def plot_D(data_no):
	#dataD = load_data_D(data_no)
	dataD = load_file(get_filename('corrD', data_no), data_shape = (4, -1), data_type = 'double')
	D_label = ['D1', 'D2', 'DA', 'DB']
	set_n = 4
	for i in range(set_n):
		plot_data(dataD[i], D_label[i], 0)
	#plot_data(dataD[2] + dataD[3], 'DA + DB')

def plot_M(data_no):
	dataM = load_file(get_filename('corrM', data_no), data_shape = (1, -1), data_type = 'double')
	plot_data(dataM, 'M(r)', 1)

def load_data_no():
	data_no = 1559632808
	sys_args = sys.argv
	if (len(sys_args) > 1):
		data_no = int(sys_args[1])
	return data_no

def load_update_size(data_no):
	data_us = load_file(get_filename('update', data_no), data_shape = (1, -1), data_type = 'int')
	return data_us

data_no = load_data_no()
update_size = load_update_size(data_no)
print(update_size)
print(np.average(update_size))
print(np.min(update_size))


if (plot_M_flag):
	plot_M(data_no)
if (plot_D_flag):
	plot_D(data_no)

plt.legend()
plt.show()