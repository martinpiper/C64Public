import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
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
        Color posCol = new Color(0, 128, 0);
        Color negCol = new Color(128, 0, 0);

        int ys = 0;
        final int kZEntries = 128;
        final double scale = 96.0f;
        final double kMultiplier = 8;
        final double nearPoint = 8.0f * kMultiplier;
        final double nearPointMinus1 = 8.0f * kMultiplier;
        final double farPoint = 128.0f * kMultiplier;
        final double zTabIndexScale = 8.0f;

        int zTab[] = new int[(int)(farPoint * zTabIndexScale)];
        int lastZ = (int)(0 * zTabIndexScale);   // Deliberate use of zTabIndexScale
//        for (double z = nearPoint; z < farPoint; z += farPoint / kZEntries) {
        for (double z = nearPoint; z < farPoint; z += (z / kMultiplier / 5)) {
            double safeDistance = 0.75f * ((z - nearPoint) / kMultiplier);
            int iSafeDistance = (int)(safeDistance / 16.0f);
            iSafeDistance *= 16;    // Avoids fractions of lines
            for (double x = -5120.0f; x < 5120.0f; x += 0.1f) {
                int xs = (int) ((480/2) + ((x * scale) / z));
                if (xs >= iSafeDistance && xs < (480 - iSafeDistance)) {
                    // Strips, including pixel rounding adjustment 0.5
                    if ((((int) (x - 0.5f)) & 64) != 0) {
                        image.setRGB(xs, ys, posCol.getRGB());
                    } else {
                        image.setRGB(xs, ys, negCol.getRGB());
                    }
                }
            }

            // Fill the reverse lookup table
            int nextZ = (int)(z * zTabIndexScale);
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
        final int yAdjust = 96; // Don't want to be so close to the ground
        for (int y = 0 ; y < 128 ; y++) {
            BufferedOutputStream outData = new BufferedOutputStream(new FileOutputStream("hor/HeightToRows" + Integer.toString(y) + ".hor"));
            int realY = y + yAdjust;
            int yTab[] = new int[kMaxRows];
            Arrays.fill(yTab,-1);
            int yTabZ[] = new int[kMaxRows];
            Arrays.fill(yTabZ,-1);
            int least = -1;
            int leastZ = -1;
//            for (double z = nearPoint; z < farPoint; z += farPoint / kZEntries) {
            for (double z = nearPoint; z < farPoint; z += (z / kMultiplier / 25)) {
                int yr = (int) ((realY * scale) / z);
                if (yr >= 0 && yr < yTab.length) {
                    int rowZ = zTab[(int) (z * zTabIndexScale)];
                    if (least < 0 || rowZ < least) {
                        least = rowZ;
//                        leastZ = (int) ((z - nearPoint) / kMultiplier);
                        leastZ = (int) ((z - nearPointMinus1) / kMultiplier);
//                        leastZ = (int) (z / kMultiplier);   // This produces an interesting bending effect
                    }
                    for (int i=0 ; i <= yr ; i++) {
                        yTab[i] = rowZ;
//                        yTabZ[i] = (int) ((z - nearPoint) / kMultiplier);
                        yTabZ[i] = (int) ((z - nearPointMinus1) / kMultiplier);
//                        yTabZ[i] = (int) (z / kMultiplier);   // This produces an interesting bending effect
                    }
                }
            }

            // Fill unused entries
            for (int i=0 ; i < yTab.length ; i++) {
                if (yTab[i] < 0) {
                    yTab[i] = least;
                    yTabZ[i] = leastZ;
                }
            }
            // Final far horizon check and change to transparent line
            for (int i=0 ; i < yTab.length ; i++) {
                if (yTab[i] >= kMaxRows) {
                    yTab[i] = 129;
                    yTabZ[i] = 0;
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
            for (int i=0 ; i < yTab.length ; i++) {
                outData.write((((0x300 + yTab[i] - (128 + i)) & 0xffff)));
            }
            for (int i=0 ; i < yTab.length ; i++) {
                outData.write((((0x300 + yTab[i] - (128 + i)) & 0xffff))>>8);
            }

            // Output the Z value for each screen row
            System.out.println("ZedToRowsRebased" + y);
            System.out.print("\t!by " + (yTabZ[0] & 0xff) );
            for (int i=1 ; i < yTabZ.length ; i++) {
                System.out.print(" , " + (yTabZ[i] & 0xff) );
            }
            System.out.println();
            for (int i=0 ; i < yTab.length ; i++) {
                outData.write(yTabZ[i] & 0xff);
            }

            outData.close();
        }

        panel.repaint();
        frame.repaint();
        frame.getContentPane().repaint();
        Thread.sleep(1);
//        frame.dispose();

    }
}
