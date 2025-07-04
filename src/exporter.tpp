#include "exporter.hpp"

// Code for exporting images to bitmap and CSV formats mainly from 'Cullen Fluffy Jennings' answer at 
//      https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
template<typename ArrayType>
void Exporter<ArrayType>::toBitmap(const std::string& filename) const
{
    assert(m_image.rows() > 0 && m_image.cols() > 0 && "The image must not be empty");

    // Image dimensions
    const int WIDTH = m_width;
    const int HEIGHT = m_height;

    // Bitmap header fields
    unsigned int headers[13];
    int extrabytes;
    int paddedsize;

    extrabytes = 4 - ((WIDTH * 3) % 4);             // How many bytes of padding to add to each
                                                    // horizontal line - the size of which must
                                                    // be a multiple of 4 bytes.
    if (extrabytes == 4){ extrabytes = 0; }     // If the size is already a multiple of 4, no padding is needed.

    paddedsize = ((WIDTH * 3) + extrabytes) * HEIGHT;

    uint8_t file_header[14] = { 
        'B', 'M', // Bitmap identifier
        0, 0, 0, 0, // bfSize (whole file size, to be filled later)
        0, 0, // bfReserved1 (both reserved, set to 0)
        0, 0, // bfReserved2 (both reserved, set to 0)
        14+40, 0, 0, 0, // bfOffbits (offset to the pixel data, 14 bytes for file header + 40 bytes for info header)
    };

    uint8_t info_header[40] = { 
        40, 0, 0, 0, // biSize (size of this header in bytes)
        0, 0, 0, 0, // biWidth Width of the image in pixel (to be filled later)
        0, 0, 0, 0, // biHeight Height of the image in pixel (to be filled later)
        1, 0, // biPlanes (Number of color planes, must be 1)
        24, 0, // biBitCount (Number of bits per pixels, 24 bits for RGB)
        0, 0, 0, 0, // biCompression (0 for no compression)
        0, 0, 0, 0, // biSizeImage (Image bit size, to be filled later)
        0, 0, 0, 0, // biXPelsPerMeter (horizontal resolution, pixels per meter, can be set to 0)
        0, 0, 0, 0, // biYPelsPerMeter (vertical resolution, pixels per meter, can be set to 0)
        0, 0, 0, 0, // biClrUsed (no color palette used)
        0, 0, 0, 0 // biClrImportant (all colors are important)
    };

    // Fill in the file header
    unsigned int file_size = paddedsize + 54; // 54 bytes for the headers
    file_header[2] = file_size & 0xFF; // bfSize (whole file size)
    file_header[3] = (file_size >> 8) & 0xFF;
    file_header[4] = (file_size >> 16) & 0xFF;
    file_header[5] = (file_size >> 24) & 0xFF;

    // Fill in the info header
    info_header[4] = WIDTH & 0xFF; // biWidth
    info_header[5] = (WIDTH >> 8) & 0xFF;
    info_header[6] = (WIDTH >> 16) & 0xFF;
    info_header[7] = (WIDTH >> 24) & 0xFF;

    info_header[8] = HEIGHT & 0xFF; // biHeight
    info_header[9] = (HEIGHT >> 8) & 0xFF;
    info_header[10] = (HEIGHT >> 16) & 0xFF;
    info_header[11] = (HEIGHT >> 24) & 0xFF;

    info_header[20] = paddedsize & 0xFF; // biSizeImage
    info_header[21] = (paddedsize >> 8) & 0xFF;
    info_header[22] = (paddedsize >> 16) & 0xFF;
    info_header[23] = (paddedsize >> 24) & 0xFF;

    // Open the file for writing
    std::ofstream outfile(filename, std::ios::binary | std::ios::trunc);
    if (!outfile.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    // Write the file header
    outfile.write(reinterpret_cast<char*>(file_header), sizeof(file_header));

    // Write the info header
    outfile.write(reinterpret_cast<char*>(info_header), sizeof(info_header));

    // Write the pixel data
    int x, y;
    int red, green, blue;
    uint8_t pixel[3]; // Array to hold the pixel color values
    uint8_t pad[3] = {0,0,0}; // Padding bytes for each row
    
    // BMP image format is written from bottom to top...
    for (y = HEIGHT - 1; y >= 0; y--) {
        for (x = 0; x <= WIDTH - 1; x++) {
            // Get the pixel color values
            int linear_id = y * WIDTH + x;

            red = m_image(linear_id, 0);
            green = m_image(linear_id, 1);
            blue = m_image(linear_id, 2);

            if (red > 255) red = 255; if (red < 0) red = 0;
            if (green > 255) green = 255; if (green < 0) green = 0;
            if (blue > 255) blue = 255; if (blue < 0) blue = 0;

            pixel[0] = static_cast<uint8_t>(blue);  // Blue channel
            pixel[1] = static_cast<uint8_t>(green); // Green channel
            pixel[2] = static_cast<uint8_t>(red);   // Red channel

            // Write the pixel data to the file
            outfile.write(reinterpret_cast<char*>(pixel), sizeof(pixel));
        }
        outfile.write(reinterpret_cast<char*>(pad), extrabytes); // Write padding bytes if needed
    }
    outfile.close();

    std::cout << "Render saved to " << filename << std::endl;
}

template<typename ArrayType>
void Exporter<ArrayType>::toCSV(const std::string& filename) const
{
    std::ofstream outfile(filename);

    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            int linear_id = i * m_width + j;
            int color = (m_image(linear_id, 0) + 
                        m_image(linear_id, 1) + 
                        m_image(linear_id, 2)) / 3;
            
            outfile << color; // Write the average color value
            if (j < m_image.cols() - 1) {
                outfile << ", ";
            }
        }
        outfile << std::endl;
    }
    outfile.close();

    std::cout << "Render saved to " << filename << std::endl;
}