from pylearn2.utils import serial
import sys
_, model_path = sys.argv
model = serial.load(model_path)
space = model.generator.get_output_space()
from pylearn2.config import yaml_parse
from pylearn2.datasets import dense_design_matrix
from pylearn2.gui.patch_viewer import PatchViewer
import numpy as np

dataset = yaml_parse.load(model.dataset_yaml_src)
if dataset.view_converter is None:
    dataset.view_converter = dense_design_matrix.DefaultViewConverter((32, 32, 3), dataset.axes)

rows = 4
sample_cols = 5

# For some reason format_as from VectorSpace is not working right
topo_samples = model.generator.sample(rows * sample_cols).eval()
samples = dataset.get_design_matrix(topo_samples)
print 'Original shape:', samples.shape
dataset.view_converter.axes = ['b', 0, 1, 'c']
topo_samples = dataset.get_topological_view(samples)

pv = PatchViewer(grid_shape=(rows, sample_cols + 1), patch_shape=(32,32),
        is_color=True)
scale = np.abs(samples).max()

X = dataset.X
topo = dataset.get_topological_view()
for i in xrange(samples.shape[0]):
    topo_sample = topo_samples[i, :, :, :]
    print topo_samples.shape, topo_sample.shape
    print topo_sample.min(), topo_sample.max(), topo_sample.shape
    pv.add_patch(topo_sample / scale, rescale=False)

    if (i +1) % sample_cols == 0:
        sample = samples[i, :]
        dists = np.square(X - sample).sum(axis=1)
        j = np.argmin(dists)
        match = topo[j]
        print 'Nearest data point:', match.min(), match.max(), match.shape
        pv.add_patch(match, rescale=True, activation=1)

pv.show()
