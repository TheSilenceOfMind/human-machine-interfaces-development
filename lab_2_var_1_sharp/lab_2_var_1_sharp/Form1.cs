using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace lab_2_var_1_sharp
{
    public partial class Form1 : Form
    {
        Bitmap bmp;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bmp = (Bitmap)pictureBox1.Image;
            Bitmap newBmp = new Bitmap(bmp.Width, bmp.Height, PixelFormat.Format24bppRgb);
            using (Graphics gr = Graphics.FromImage(newBmp))
            {
                gr.DrawImage(bmp, 0, 0, bmp.Width, bmp.Height);
            }

            SaveFileDialog savedialog = new SaveFileDialog();
            savedialog.Title = "Save as ...";
            savedialog.OverwritePrompt = true;
            savedialog.CheckPathExists = true;
            savedialog.Filter = "Image Files(*.BMP)|*.BMP";
            savedialog.ShowHelp = true;
            if (savedialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    bmp.Save(savedialog.FileName, System.Drawing.Imaging.ImageFormat.Bmp);
                }
                catch
                {
                    MessageBox.Show("Can't save the image", "Error!",
                        MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void openBMPFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Filter = "Image Files(*.BMP;)|*.BMP;";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    bmp = new Bitmap(dialog.FileName);
                    pictureBox1.Image = bmp;
                    pictureBox1.Invalidate();
                    actionsToolStripMenuItem.Enabled = true;
                    saveAsToolStripMenuItem.Enabled = true;
                }
                catch
                {
                    MessageBox.Show("Can't display selected image!",
                    "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void quitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void swapRAndBToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ImageAttributes imageAttributes = new ImageAttributes();

            float[][] colorMatrixElements = {
                                                new float[] {0, 0, 1, 0, 0}, // R
                                                new float[] {0, 1, 0, 0, 0}, // G
                                                new float[] {1, 0, 0, 0, 0}, // B
                                                new float[] {0, 0, 0, 1, 0}, // Aplha
                                                new float[] {0, 0, 0, 0, 1}  // w
                                            };

            ColorMatrix colorMatrix = new ColorMatrix(colorMatrixElements);

            imageAttributes.SetColorMatrix(colorMatrix, ColorMatrixFlag.Default, ColorAdjustType.Bitmap);
            Graphics graphics = Graphics.FromImage(bmp);
            graphics.DrawImage(bmp, new Rectangle(0, 0, bmp.Size.Width, bmp.Size.Height), 0, 0, bmp.Size.Width, bmp.Size.Height, GraphicsUnit.Pixel, imageAttributes);

            pictureBox1.Image = bmp;
        }
    }
}
