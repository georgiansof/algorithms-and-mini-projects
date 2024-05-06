from PIL import Image
import os

# Directory containing the images
input_dir = './'

# Output directory where resized images will be saved
output_dir = './output/'

# Desired width and height for resized images
target_size = (250, 250)

# Ensure the output directory exists, create if not
os.makedirs(output_dir, exist_ok=True)

# List all files in the input directory
files = os.listdir(input_dir)

# Iterate through each file
for filename in files:
    # Check if the file is a JPG file
    if filename.lower().endswith('.jpg') or filename.lower().endswith('.jpeg'):
        # Construct the full paths for input and output files
        input_path = os.path.join(input_dir, filename)
        output_path = os.path.join(output_dir, filename)

        # Open the image file
        with Image.open(input_path) as img:
            # Resize the image
            resized_img = img.resize(target_size)

            # Save the resized image as JPG
            resized_img.save(output_path, format='JPEG')

            print(f'Resized: {input_path} -> {output_path}')
