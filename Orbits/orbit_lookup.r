system("python2 kepler.py")

# LOAD ALL THE FILES
trajectories = list()
for(i in 1:121)
{
  v = 0.4 + 0.005*(i-1)
  filename = paste('orbits', format(round(v, 3), nsmall=3), sep='')
  filename = paste(filename, '.dat', sep='')
  trajectories[[i]] = as.matrix(read.table(filename))
}

evaluate = function(arg_to_sin, v=0.75, viewing_angle=0.)
{
  # Which trajectory to use?
  orbit = floor((v - 0.4)/0.005) + 1
  N = dim(trajectories[[orbit]])[1]

  radial_velocity = cos(viewing_angle)*trajectories[[orbit]][,1] + sin(viewing_angle)*trajectories[[orbit]][,2]
  radial_velocity = radial_velocity/max(abs(radial_velocity))
  radial_velocity = (radial_velocity - min(radial_velocity))/(max(radial_velocity) - min(radial_velocity))
  radial_velocity = 2*radial_velocity - 1
  closest_to_max = which(radial_velocity == max(radial_velocity))[1]

  cc = 2*pi*(closest_to_max - 1)/N
  index = 1 + N*((arg_to_sin + cc) %% (2*pi))/(2*pi)

  return(radial_velocity[index])
}

t = seq(0, 10, by=0.01)
y = evaluate(2*pi*t/5 + 2.2, v=sqrt(1. - 0.5), viewing_angle=-2.7)
plot(t, y)

data = as.matrix(read.csv("kepler.txt", sep=" "))
lines(data[,1], data[,2])

