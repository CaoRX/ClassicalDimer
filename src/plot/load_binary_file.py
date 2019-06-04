import numpy as np 
import struct

def load_file(filename, data_shape = (1, -1), data_type = 'double'):
	type_dict = {'int': 'i', 'double': 'd'}
	file_path = '../../build/data/' + filename
	data_file = open(file_path, mode = 'rb')
	file_contents = data_file.read()
	ret = struct.iter_unpack(type_dict[data_type], file_contents)
	ret = np.array([a[0] for a in ret])
	if (data_shape[0] == 1) and (data_shape[1] == -1):
		return ret

	ret = np.reshape(ret, data_shape)
	return ret 

def get_filename(file_type, file_no):
	return file_type + str(file_no) + '.dat'