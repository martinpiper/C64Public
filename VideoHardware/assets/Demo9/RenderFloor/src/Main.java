import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import javax.imageio.ImageIO;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.plaf.ColorUIResource;

public class Main {
    public static void main(String args[]) throws InterruptedException, IOException {
        JFrame frame = new JFrame("JFrame Example");
        ImagePanel panel = new ImagePanel();
        frame.add(panel);

        frame.setSize(panel.getWidth() + 20, panel.getHeight() + 50);
        frame.setLocationRelativeTo(null);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);

        BufferedImage image = panel.getImage();
        Graphics graphics = image.getGraphics();
        graphics.setColor(new Color(255, 0, 255));
        graphics.fillRect(0, 0, image.getWidth(), image.getHeight());
        graphics.setColor(Color.BLACK);
//        graphics.fillRect(0, 112, 480, 224);
        Color posCol[] = new Color[7];
        Color negCol[] = new Color[7];
        // 7 colour shades for each, meaning we should get maximum 14 + 1 (black) + transparent colour
        for (int i = 0; i < 7; i++) {
            posCol[i] = new Color(0, 128, i * 32);
            negCol[i] = new Color(128, 0, i * 32);
        }

        int ys = 0;
        final int kZEntries = 128;
        final double scale = 96.0f;
        final double kMultiplier = 8;
        final double nearPoint = 8.0f * kMultiplier;
        final double farPoint = 128.0f * kMultiplier;

        int zTab[] = new int[(int)farPoint];
        int lastZ = 0;
        for (double z = nearPoint; z < farPoint; z += farPoint / kZEntries) {
            for (double x = -5120.0f; x < 5120.0f; x += 0.1f) {
                int xs = (int) ((480/2) + ((x * scale) / z));
                if (xs >= 0 && xs < 480) {
                    int zc = ((int) z) & 63;
                    zc = zc * 7 / 64;

                    // Strips
                    if ((((int) x) & 64) != 0) {
                        image.setRGB(xs, ys, posCol[0].getRGB());
                    } else {
                        image.setRGB(xs, ys, negCol[0].getRGB());
                    }
                }
            }

            // Fill the reverse lookup table
            int nextZ = (int)z;
            while (lastZ <= nextZ) {
                zTab[lastZ] = ys;
                lastZ++;
            }
            ys++;
        }

        while (lastZ < zTab.length) {
            zTab[lastZ] = ys-1;
            lastZ++;
        }

        ImageIO.write(image,"png", new File("out/floor.png"));

        final int kMaxRows = 112;
        final int yAdjust = 32;
        for (int y = 0 ; y < 128 ; y++) {
            int realY = y + yAdjust;
            int yTab[] = new int[kMaxRows];
            Arrays.fill(yTab,-1);
            int least = -1;
            for (double z = nearPoint; z < farPoint; z += farPoint / kZEntries) {
                int yr = (int) ((realY * scale) / z);
                if (yr >= 0 && yr < yTab.length) {
                    int rowZ = zTab[(int) z];
                    if (least < 0 || rowZ < least) {
                        least = rowZ;
                    }
                    for (int i=0 ; i <= yr ; i++) {
                        yTab[i] = rowZ;
                    }
                }
            }

            // Fill unused entries
            for (int i=0 ; i < yTab.length ; i++) {
                if (yTab[i] < 0) {
                    yTab[i] = least;
                }
            }

            System.out.println("HeightToRows" + y);
            System.out.print("\t!by " + yTab[0]);
            for (int i=1 ; i < yTab.length ; i++) {
                System.out.print(" , " + yTab[i]);
            }
            System.out.println();

            // Output rows index minus the screen line
            System.out.println("HeightToRowsInv" + y);
            System.out.print("\t!word " + ((0x300 + yTab[0] - 128) & 0xffff) );
            for (int i=1 ; i < yTab.length ; i++) {
                System.out.print(" , " + ((0x300 + yTab[i] - (128 + i)) & 0xffff) );
            }
            System.out.println();
        }

        panel.repaint();
        frame.repaint();
        frame.getContentPane().repaint();
        Thread.sleep(1);
//        frame.dispose();

    }
}
