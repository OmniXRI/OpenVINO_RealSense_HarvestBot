# -*- coding: utf-8 -*-
# 參考文獻 https://www.twblogs.net/a/5bfae8b4bd9eee7aed32c9bb

from keras.models import load_model
import tensorflow as tf
import os 
import os.path as osp
from keras import backend as K

from yolo3.model import yolo_body
from keras.layers import Input
#路徑引數
input_path = 'C:/Users/jack_/my_yolo3/model_data/' #待轉換*.h5路徑
weight_file = 'trained_weights_final.h5' #待轉換*.h5檔名
weight_file_path = osp.join(input_path,weight_file) #完整待轉換含路徑檔案名稱
output_graph_name = weight_file[:-3] + '.pb' #轉換成TensorFlow格式後檔名*.pb
#轉換函式
def h5_to_pb(h5_model,output_dir,model_name,out_prefix = "output_",log_tensorboard = True):
    if osp.exists(output_dir) == False:
        os.mkdir(output_dir)
    out_nodes = []
    for i in range(len(h5_model.outputs)):
        out_nodes.append(out_prefix + str(i + 1))
        tf.identity(h5_model.output[i],out_prefix + str(i + 1))
    sess = K.get_session()
    from tensorflow.python.framework import graph_util,graph_io
    init_graph = sess.graph.as_graph_def()
    main_graph = graph_util.convert_variables_to_constants(sess,init_graph,out_nodes)
    graph_io.write_graph(main_graph,output_dir,name = model_name,as_text = False)
    if log_tensorboard:
        from tensorflow.python.tools import import_pb_to_tensorboard
        import_pb_to_tensorboard.import_to_tensorboard(osp.join(output_dir,model_name),output_dir)
#輸出路徑
output_dir = osp.join(os.getcwd(),"trans_model")
#載入模型
#h5_model = load_model(weight_file_path)
num_classes = 1
h5_model = yolo_body(Input(shape=(None, None, 3)), 3, num_classes) 
h5_model.load_weights(weight_file_path)

h5_to_pb(h5_model,output_dir = output_dir,model_name = output_graph_name)
print('model saved')
