# Signal Processing in Numpy and Tensorflow

Similar to the webscraper, I decided to focus on building and tidying up my own dataset. So I found the [Facial Expression Comparison](https://research.google/tools/datasets/google-facial-expression/) which provides images as links to Flickr photos in a `.csv`. The images are therefore unedited, but the face is given as a pair of normalised coordinates in the `.csv` file so the face can be cropped out of the image.

There are some comments and titles throughout the notebook explaining what is happening where. It involves some small experiments on single images and then the same operation performed in batch on the dataset. A sample of images are included in the repo so the notebook works fine in Github.

It was a good exercise to get used to Numpy arrays as cropping the face was done by cutting a chunk out of the Numpy array:

```python
h = img.shape[0]
w = img.shape[1]
tl_x = int(round(data['b_tl_x'][0]*w))
br_x = int(round(data['b_br_x'][0]*w))
tl_y = int(round(data['b_tl_y'][0]*h))
br_y = int(round(data['b_br_y'][0]*h))

head_width = br_x - tl_x
head_height = br_y - tl_y

face_crop = data[tl_y:br_y, tl_x:br_x]
```

I experimented with some Scipy image filtering techniques like so:

```python
from scipy import misc, ndimage
face = misc.face(gray=True).astype(float)

blurred_f = ndimage.gaussian_filter(drummer_face, 2)
filter_blurred_f = ndimage.gaussian_filter(blurred_f, 1)
alpha = 3
sharpened = blurred_f + alpha * (blurred_f - filter_blurred_f)
```

OpenCV was then used to resize the images so that they are all the same:

```python
import cv2

res = cv2.resize(face_data[0], dsize=(300, 140), interpolation=cv2.INTER_CUBIC)
```

---

## To run:

```
conda create -n img-dataset
conda activate img-dataset
jupyter-notebook
```

However the notebook can be seen in Github [here](https://github.com/joshmurr/HAND-IN-msc-coding-2-in-class-assignments/blob/master/signal_processing_w_np_tf/Numpy%20%7C%20Datasets.ipynb).
