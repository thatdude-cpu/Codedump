import java.awt.Graphics;
import java.applet.Applet;

public class HelloWorld extends Applet
{
 String myString ="Das ist mein Hallo-Text";

 public void paint(Graphics g)
 {
  g.drawString(myString, 25, 20);
 }
 public void setString(String aString)
 {
  myString = aString;
  repaint();
 }
}