library(tiff)
library(waved)
# load the image
p <- paste(getwd(),"/some_image.tif", sep="")
img <- readTIFF(p)

# if R support plotting image, let's plot
if (exists("rasterImage")) 
{
  plot(1:2, type="n")
  rasterImage(img, 1, 1, 2, 2)
}

if (nrow(img) == ncol(img))
{
  # create a 'noise'-function
  t = seq(from=0, to=nrow(img), len=nrow(img))
  g = besselJ(t, 0);
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
  
  if (exists("rasterImage")) 
  {
    plot(1:2, type="n")
    rasterImage(img, 1, 1, 2, 2)
  }
  
  # save image to file
  writeTIFF(img, where="some_image_waved.tif");
}

