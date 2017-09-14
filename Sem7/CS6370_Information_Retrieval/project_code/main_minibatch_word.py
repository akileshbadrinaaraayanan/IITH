#pylint: skip-file
import time
import sys
import numpy as np
import theano
import theano.tensor as T
from utils_pg import *
from rnn_word import *
import data
import sys
import argparse
from time import strftime


parser = argparse.ArgumentParser()
parser.add_argument("--gpu", default=3, type=int)
parser.add_argument("--hid", default=1000, type=int)
parser.add_argument("--bsize", default=128, type=int)
parser.add_argument("--epoch", default=1000, type=int)
parser.add_argument("--path", default="sentence_10_full_article.txt")
parser.add_argument("--opt", default="nesterov_momentum")
parser.add_argument("--model")

args = parser.parse_args(sys.argv[1:])
use_gpu(args.gpu) # -1:cpu; 0,1,2,..: gpu

e = 0.01
lr = 0.1
drop_rate = 0.
batch_size = args.bsize
hidden_size = [args.hid]
# try: gru, lstm
cell = "lstm"
# try: sgd, momentum, rmsprop, adagrad, adadelta, adam, nesterov_momentum
#optimizer = "adam" 
optimizer = args.opt

seqs, i2w, w2i, data_xy = data.word_sequence(args.path, batch_size)
dim_x = len(w2i)
dim_y = len(w2i)
num_sents = data_xy[0][3]
print "#features = ", dim_x, "#labels = ", dim_y

print "compiling..."
model = RNN(dim_x, dim_y, hidden_size, cell, optimizer, drop_rate, num_sents)
print "training..."
start = time.time()
g_error = 9999.9999
if args.model is None:
    for i in xrange(args.epoch):
        error = 0.0
        in_start = time.time()
        
        for batch_id, xy in data_xy.items():
            X = xy[0]
            mask = xy[2]
            local_batch_size = xy[3]
            cost, sents = model.train(X, mask, lr, local_batch_size)
            error += cost
        #in_time = time.time() - in_start

            if (i+1)%200==0:
                f = open('data/outputword_1_'+args.path.split('/')[-1], 'w')
                for s in xrange(int(sents.shape[1] / dim_y)):
                    xs = sents[:, s * dim_y : (s + 1) * dim_y]
                    for w_i in xrange(xs.shape[0]):
                        w = i2w[np.argmax(xs[w_i, :])]
                        if w == "<eoss>":
                            break
                        f.write(w+' ')
                    f.write("\n")
                f.close()
                
        in_time = time.time() - in_start
        #if(i+1)%200==0:
         #   lr*=0.2
        error /= len(data_xy);
        if error < g_error:
            g_error = error

        print "Iter = " + str(i) + ", Error = " + str(error) + ", Time = " + str(in_time)
        if error <= e:
            break

    print "Finished. Time = " + str(time.time() - start)

    print "save model..."
    save_time = strftime("%H:%M:%S", time.localtime())
    save_model("./model/hed_word_1_"+save_time+".model", model)

else:
    load_model(args.model, model)
f = open('data/outputword_1_'+args.path.split('/')[-1], 'w')
for batch_id, xy in data_xy.items():
    X = xy[0]
    mask = xy[2]
    local_batch_size = xy[3]
    out = model.predict(X, mask, local_batch_size)[0]
    for s in xrange(int(out.shape[1] / dim_y)):
        xs = out[:, s * dim_y : (s + 1) * dim_y]
        for w_i in xrange(xs.shape[0]):
            w = i2w[np.argmax(xs[w_i, :])]
            if w == "<eoss>":
                break
            f.write(w+' ')
        f.write("\n")

f.close()
