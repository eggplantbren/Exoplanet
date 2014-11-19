from pylab import *
import os

data = loadtxt('fake_data_like_nuoph.txt')
truth = loadtxt('fake_data_like_nuoph.truth')
posterior_sample = atleast_2d(loadtxt('posterior_sample.txt'))

hist(posterior_sample[:,1007], 100)
xlabel('Number of Planets')
show()

T = posterior_sample[:,1008:1018]
A = posterior_sample[:,1018:1028]
E = posterior_sample[:,1038:1048]
which = T != 0
T = T[which].flatten()
A = A[which].flatten()
E = E[which].flatten()
# Trim
#s = sort(T)
#left, middle, right = s[0.25*len(s)], s[0.5*len(s)], s[0.75*len(s)]
#iqr = right - left
#s = s[logical_and(s > middle - 5*iqr, s < middle + 5*iqr)]

hist(T/log(10.), 300, alpha=0.5)
xlabel(r'$\log_{10}$(Period/days)')
xlim([0, 4])
for i in xrange(1008, 1008 + int(truth[1007])):
  axvline(truth[i]/log(10.), color='r')
show()

subplot(2,1,1)
plot(T/log(10.), A, 'b.', markersize=1)
xlim([0, 4])
ylabel('Amplitude (m/s)')
plot(truth[1008:1008 + int(truth[1007])]/log(10.), truth[1018:1018 + int(truth[1007])], 'ro')

subplot(2,1,2)
plot(T/log(10.), E, 'b.', markersize=1)
xlim([0, 4])
xlabel(r'$\log_{10}$(Period/days)')
ylabel('Ellipticity')
plot(truth[1008:1008 + int(truth[1007])]/log(10.), truth[1038:1038 + int(truth[1007])], 'ro')
show()

data[:,0] -= data[:,0].min()
t = linspace(data[:,0].min(), data[:,0].max(), 1000)

saveFrames = False # For making movies
if saveFrames:
  os.system('rm Frames/*.png')

ion()
for i in xrange(0, posterior_sample.shape[0]):
  hold(False)
  errorbar(data[:,0], data[:,1], fmt='b.', yerr=data[:,2])
  hold(True)
  plot(t, posterior_sample[i, 0:1000], 'r')
  xlim([-0.05*data[:,0].max(), 1.05*data[:,0].max()])
  ylim([-1.5*max(abs(data[:,1])), 1.5*max(abs(data[:,1]))])
  #axhline(0., color='k')
  xlabel('Time (days)', fontsize=16)
  ylabel('Radial Velocity (m/s)', fontsize=16)
  draw()
  if saveFrames:
    savefig('Frames/' + '%0.4d'%(i+1) + '.png', bbox_inches='tight')
    print('Frames/' + '%0.4d'%(i+1) + '.png')


ioff()
show()
