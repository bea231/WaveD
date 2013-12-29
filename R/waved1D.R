library(waved)
# load example data
data <- waved.example(TRUE,FALSE)
# make the algorithm
doppler.wvd=WaveD(data$doppler.noisy, data$g)
# plotting the results
plot(data$t, data$doppler, type="l", main="Original", xlab="t", ylab="doppler")
plot(data$t, data$doppler.blur, type="l", main="blurred", xlab="t", ylab="doppler")
plot(data$t, data$doppler.noisy, type="l", main="noisy", xlab="t", ylab="doppler")
plot(data$t, doppler.wvd$waved, type="l", main="waved", xlab="t", ylab="doppler")
plot(data$t, data$g, type="l", main="waved", xlab="t", ylab="doppler")

