import argparse
from PIL import Image
import os

def ppm_to_jpg(ppm_file, jpg_file):
    if not os.path.exists(ppm_file):
        print(f"Error - file not found: {ppm_file}")
        return
    if not ppm_file.lower().endswith('.ppm'):
        print(f"Error - file not PPM file: {ppm_file}")
        return
    try:
        with Image.open(ppm_file) as img:
            img.save(jpg_file, "JPEG")
    except Exception as e:
        print(f"Error - could not open .ppm or save .jpg file: {e}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Convert .ppm file to .jpg file")
    parser.add_argument("--ppm_file", help="Path to .ppm file")
    parser.add_argument("--jpg_file", help="Path to .jpg file")
    args = parser.parse_args()
    ppm_to_jpg(args.ppm_file, args.jpg_file)

