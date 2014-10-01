from pylab import *
import os

data = loadtxt('exoplanet.txt')
posterior_sample = atleast_2d(loadtxt('posterior_sample.txt'))

hist(posterior_sample[:,1007], 100)
xlabel('Number of Planets')
show()

T = posterior_sample[:,1008:1018]
T = T[T != 0].flatten()
hist(T, 200, alpha=0.5)
xlabel(r'$\ln$(Period/days)')
show()

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
