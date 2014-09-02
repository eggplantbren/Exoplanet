from pylab import *
import os

data = loadtxt('exoplanet.txt')
posterior_sample = atleast_2d(loadtxt('posterior_sample.txt'))

t = linspace(data[:,0].min(), data[:,0].max(), 1000)

saveFrames = False # For making movies
if saveFrames:
  os.system('rm Frames/*.png')

ion()
for i in xrange(0, posterior_sample.shape[0]):
  hold(False)
  plot(data[:,0], data[:,1], 'b.')
  hold(True)
  plot(t, posterior_sample[i, 0:1000], 'r')
  ylim([-1.5*max(abs(data[:,1])), 1.5*max(abs(data[:,1]))])
  axhline(0., color='k')
  xlabel('Time', fontsize=16)
  ylabel('y', fontsize=16)
  draw()
  if saveFrames:
    savefig('Frames/' + '%0.4d'%(i+1) + '.png', bbox_inches='tight')
    print('Frames/' + '%0.4d'%(i+1) + '.png')


ioff()
show()
