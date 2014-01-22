library(tiff)
library(waved)

args<-commandArgs(trailingOnly=TRUE)

# load the image
img <- readTIFF(args[1])

if (nrow(img) == ncol(img))
{
  # create a 'noise'-function
  t = seq(from=0, to=nrow(img), len=nrow(img))
  data.g = besselJ(t, 0);
  # set max-theroshold parameter
  eta = 0.01;
  
  for (k in 1:nrow(img))
  {
    # load k-th row of image (red and green components)
    r = img[k,,1]
    g = img[k,,2]   
    
    # apply WaveD method to row
    r.wvd=WaveD(r, data.g, MC=TRUE, eta=eta)
    g.wvd=WaveD(g, data.g, MC=TRUE, eta=eta)
    
    # load row components into image
    img[k,,1] = r.wvd    
    img[k,,2] = g.wvd
  }     
  # clamp the results
  img[img > 1] = 1;
  img[img < 0] = 0; 
  
  # save image to file
  writeTIFF(img, where=paste(args[1], "_waved.tif"));
}

