from pylab import *

t = linspace(0, 10, 10001)

e = 0.5		# Eccentricity
P = 5.		# Period
K = 1.		# Semi-amplitude
phi = 1.	# Longitude of ascending node
wbar = pi/2	# Longitude of periastron
omega = 2.*pi/P

# Solve for E
def update(E, t):
  E = E - (E - e*sin(E) - omega*t - phi)/(1. - e*cos(E))
  return E

ion()
hold(False)
E = omega*t + phi
for i in xrange(0, 20):
  hold(True)
  plot(t, E)
  E = update(E, t)
  title(i+1)
  draw()

ioff()
show()

E = mod(E, 2*pi)
cosf = (cos(E) - e)/(1. - e*cos(E))
f = arccos(cosf)
f[E > pi] = 2*pi - f[E > pi]
v = K*(sin(f + wbar) + e*sin(wbar))
v -= (v.min() + v.max())/2
print(v.max() - v.min())
plot(t, v)

data = empty((10001, 2))
data[:,0], data[:,1] = t, v
savetxt('kepler.txt', data)
show()

# EQUIVALENCE
# kepler.py			Old code (but using cosine alignment)
# (phi, wbar) = (0, pi/2)		(phase, viewing angle) = (0, 0)
# (phi, wbar) = 
