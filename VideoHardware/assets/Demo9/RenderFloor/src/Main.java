import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.plaf.ColorUIResource;

public class Main {
    public static void main(String args[]) throws InterruptedException {
        JFrame frame = new JFrame("JFrame Example");
        ImagePanel panel = new ImagePanel();
        frame.add(panel);

        frame.setSize(panel.getWidth() + 20, panel.getHeight() + 50);
        frame.setLocationRelativeTo(null);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);

        int imageIndex = 0;
        for (double xo = 0 ; xo < 64.0f ; xo += 64.0f / (3*3)) {
            BufferedImage image = panel.getImage();
            Graphics graphics = image.getGraphics();
            graphics.setColor(new Color(255, 0, 255));
            graphics.fillRect(0, 0, image.getWidth(), image.getHeight());
            graphics.setColor(Color.BLACK);
            graphics.fillRect(0, 112, 320, 224);
            Color posCol[] = new Color[7];
            Color negCol[] = new Color[7];
            // 7 colour shades for each, meaning we should get maximum 14 + 1 (black) + transparent colour
            for (int i = 0; i < 7; i++) {
                posCol[i] = new Color(0, 128, i * 32);
                negCol[i] = new Color(128, 0, i * 32);
            }

            // Very bad perspective correct draw as it uses a lot of overdraw...
            for (double z = 0.1f; z < 1024.0f/*4096.0f*/; z += 1.0f) {
                for (double x = -5120.0f; x < 5120.0f; x += 1.0f) {
                    double y = 128.0f;
                    final double scale = 96.0f;
                    int xs = (int) (160.5f + (((x + xo) * scale) / z));
                    int ys = (int) (112.5f + ((y * scale) / z));
                    if (xs >= 0 && xs < image.getWidth() && ys >= 0 && ys < image.getHeight()) {
                        int zc = ((int) z) & 63;
                        zc = zc * 7 / 64;

                        // Strips
                        if ((((int) x) & 64) != 0) {
                            image.setRGB(xs, ys, posCol[0].getRGB());
                        } else {
                            image.setRGB(xs, ys, negCol[0].getRGB());
                        }
/*
                        // Checkerboard
                        if ((((int) x) & 64) != 0 ^ (((int) z) & 64) != 0) {
                            image.setRGB(xs, ys, posCol[0].getRGB());
                        } else {
                            image.setRGB(xs, ys, negCol[0].getRGB());
                        }
*/
/*
                        // Checkerboard with graduations
                        if ((((int) x) & 64) != 0 ^ (((int) z) & 64) != 0) {
                            image.setRGB(xs, ys, posCol[zc].getRGB());
                        } else {
                            image.setRGB(xs, ys, negCol[zc].getRGB());
                        }
*/
                    }
                }
            }

            try {
                ImageIO.write(image,"png", new File("out/frame" + imageIndex + ".png"));
                imageIndex++;
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            panel.repaint();
            frame.repaint();
            frame.getContentPane().repaint();
            Thread.sleep(1);
        }
//        frame.dispose();
    }
}
