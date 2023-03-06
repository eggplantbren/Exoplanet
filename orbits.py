from pylab import *

for v0 in linspace(0.4, 1., 121):
	# Initial position
	pos = array([1., 0.])

	# Initial velocity
	vel = array([0., v0])

	# Timestep
	dt = 0.0005
	tfinal = 10.
	steps = int(tfinal/dt)

	keep = empty((steps, 2))

	ion()
	for i in range(0, steps):
	  phi1 = arctan2(pos[1], pos[0])

	  pos += 0.5*dt*vel
	  accel = -pos/(pos[0]**2 + pos[1]**2)**1.5
	  vel += dt*accel
	  pos += 0.5*dt*vel
	  keep[i, :] = vel

	  phi2 = arctan2(pos[1], pos[0])

#	  if i%50 == 0:
#	    hold(False)
#	    plot(keep[0:(i+1), 0], keep[0:(i+1), 1], 'bo')
#	    hold(True)
#	    plot(0, 0, 'r*')
#	    axis('equal')
#	    draw()

	  if phi1 < 0. and phi2 > 0.:
	    keep = keep[0:(i+1), :]
	    break
	 
	savetxt('Orbits/orbits' + '{v:.3f}'.format(v=v0) + '.dat', keep)
ioff()
show()

