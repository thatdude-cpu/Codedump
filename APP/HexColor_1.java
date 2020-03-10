/* my second excercise in java programming. 
   basically sgi's cedit for java... 
	added for folks using paul's addbevel (& other sane apps :)
   ...normal numeric values! look to the right. that's what those are.

   to call this file within a page, do this:
	copy my stuff to your machine
	(you'll need HexColor.java as well as the NoFlickerApplet.class)
   compile it/them
	stick this in your (otherwise properly coded...i'm a stickler for these 
	things  :) HTML file:
   <APPLET CODE="HexColor.class" WIDTH=150 HEIGHT=155>
   <PARAM NAME="TestText" VALUE="foo">
   </APPLET> 
   where "foo" is your test text. that will display on the screen

   this code is copyright (c)1995 jon madison.
   if you use it, tell where you got it from.
   thankya.

   jm@iquest.net, et. al.
 */

/* note...the following isn't good programming practice.
   i coulda made it more "modular"...cut me slack. my first
   complete java prog, eh?. :) */

// here i go...

// define the following classes:
// Rslider;
// Glslider
// Bslider;


/* another note: this could be shorter; i tried to import exactly
   what i needed for instructional purposes. */

import java.awt.Graphics;	/* any screen painting needs this */
import java.applet.Applet;	/* yer hosed if you don't */
import java.awt.*;

public class HexColor extends NoFlickerApplet
{
  private Button cbutton;

  String hx;
  String hx_arr[] = new String[3];
  String hexstr;		// foreground color text

  String hexstr_bg;		// background color text

  int r, g, b;			// foreground color values

  int rb = 255;			// background colors

  int gb = 255;
  int bb = 255;

  Color color;			// color of the text

  Color colorbg;		// color of background

  int r_width;
  int fc_width = 255;
  int bc_width = 255;
  int fc_h_offset = 300;
  int bc_h_offset = 300;
  int rfc_v_offset = 70;
  int gfc_v_offset = rfc_v_offset + 17;
  int bfc_v_offset = gfc_v_offset + 17;
  int rbc_v_offset = 140;
  int gbc_v_offset = rbc_v_offset + 17;
  int bbc_v_offset = gbc_v_offset + 17;
  int height = 7;		// height of selection bar.

  int t_width = 5;
  int t_height = 15;		// little ticker thingie

  int tgf_offset, tbf_offset;
  int tgb_offset, tbb_offset;
  int trf_offset = tgf_offset = tbf_offset = fc_h_offset;
  int trb_offset = tgb_offset = tbb_offset = (bc_h_offset + 255) - t_width;

  public void init ()
  {
    hexstr = "0x000000";
    hexstr_bg = "0xffffff";
    cbutton = new Button("Reset"); // reset button
    this.add(cbutton);
    repaint ();
  }

  public void paint (Graphics gr)
  {
    Font font = new Font ("Helvetica", Font.BOLD, 50);
    String s = getParameter ("TESTTEXT");
      gr.setFont (font);
      colorbg = new Color (rb, gb, bb);
      gr.setColor (colorbg);
      r_width = gr.getFontMetrics ().stringWidth (s) + fc_width + fc_h_offset + 20;
      gr.fillRect (0, 0, r_width, 300);
      color = new Color (r, g, b);
      gr.setColor (color);
      gr.drawString (s, 20, 55);
      font = new Font ("Courier", Font.BOLD, 24);
      gr.setFont (font);

    //time to determine which string we should be displaying:
    String rs, gs, bs;		// foreground

    if (r < 16)
        rs = "0" + Integer.toString (r, 16);
    else
        rs = Integer.toString (r, 16);
    if (g < 16)
        gs = "0" + Integer.toString (g, 16);
    else
        gs = Integer.toString (g, 16);
    if (b < 16)
        bs = "0" + Integer.toString (b, 16);
    else
        bs = Integer.toString (b, 16);

      hexstr = "fg: " + "0x" + rs + gs + bs;

    String rbs, gbs, bbs;	// background

    if (rb < 16)
        rbs = "0" + Integer.toString (rb, 16);
    else
        rbs = Integer.toString (rb, 16);
    if (gb < 16)
        gbs = "0" + Integer.toString (gb, 16);
    else
        gbs = Integer.toString (gb, 16);
    if (bb < 16)
        bbs = "0" + Integer.toString (bb, 16);
    else
        bbs = Integer.toString (bb, 16);

      hexstr_bg = "bg: " + "0x" + rbs + gbs + bbs;

      gr.drawString (hexstr, 20, 105);
      gr.drawString (hexstr_bg, 20, 145);

    // draw the scrollthingie selectors.

      font = new Font ("Helvetica", Font.PLAIN, 16);
      gr.setFont (font);

    //foreground scrollthingies
      color = new Color (0, 0, 0);
      gr.setColor (color);
      gr.drawString ("FG", fc_h_offset - 35, gfc_v_offset + 5);
      color = new Color (255, 0, 0);
      gr.setColor (color);
      gr.fillRect (fc_h_offset, rfc_v_offset, fc_width, height);
      color = new Color (0, 0, 0);
      gr.setColor (color);
      gr.drawRect (fc_h_offset, rfc_v_offset, fc_width, height);
      gr.fillRect (trf_offset, rfc_v_offset - 3, t_width, t_height);
      gr.drawString (Integer.toString (r), fc_h_offset + 257, rfc_v_offset + 5);
      color = new Color (0, 255, 0);
      gr.setColor (color);
      gr.fillRect (fc_h_offset, gfc_v_offset, fc_width, height);
      color = new Color (0, 0, 0);
      gr.setColor (color);
      gr.drawRect (fc_h_offset, gfc_v_offset, fc_width, height);
      gr.fillRect (tgf_offset, gfc_v_offset - 3, t_width, t_height);
      gr.drawString (Integer.toString (g), fc_h_offset + 257, gfc_v_offset + 5);
      color = new Color (0, 0, 255);
      gr.setColor (color);
      gr.fillRect (fc_h_offset, bfc_v_offset, fc_width, height);
      color = new Color (0, 0, 0);
      gr.setColor (color);
      gr.drawRect (fc_h_offset, bfc_v_offset, fc_width, height);
      gr.drawString (Integer.toString (b), fc_h_offset + 257, bfc_v_offset + 5);
      gr.fillRect (tbf_offset, bfc_v_offset - 3, t_width, t_height);

    //background scrollthingies
      color = new Color (0, 0, 0);
      gr.setColor (color);
      gr.drawString ("BG", bc_h_offset - 35, gbc_v_offset + 5);
      color = new Color (255, 0, 0);
      gr.setColor (color);
      gr.fillRect (bc_h_offset, rbc_v_offset - 5, bc_width, height);
      color = new Color (0, 0, 0);
      gr.setColor (color);
      gr.drawRect (bc_h_offset, rbc_v_offset - 5, bc_width, height);
      gr.fillRect (trb_offset, rbc_v_offset - 8, t_width, t_height);
      gr.drawString (Integer.toString (rb), bc_h_offset + 257, rbc_v_offset + 2);
      color = new Color (0, 255, 0);
      gr.setColor (color);
      gr.fillRect (bc_h_offset, gbc_v_offset - 5, bc_width, height);
      color = new Color (0, 0, 0);
      gr.setColor (color);
      gr.drawRect (bc_h_offset, gbc_v_offset - 5, bc_width, height);
      gr.fillRect (tgb_offset, gbc_v_offset - 8, t_width, t_height);
      gr.drawString (Integer.toString (gb), bc_h_offset + 257, gbc_v_offset + 2);
      color = new Color (0, 0, 255);
      gr.setColor (color);
      gr.fillRect (bc_h_offset, bbc_v_offset - 5, bc_width, height);
      color = new Color (0, 0, 0);
      gr.setColor (color);
      gr.drawRect (bc_h_offset, bbc_v_offset - 5, bc_width, height);
      gr.fillRect (tbb_offset, bbc_v_offset - 8, t_width, t_height);
      gr.drawString (Integer.toString (bb), bc_h_offset + 257, bbc_v_offset + 2);
      colorbg = new Color (rb, gb, bb);		// set background according to foreground

  }
  public boolean mouseDown (Event evt, int x, int y)
  {
    if(evt.target == cbutton){
      	r = g = b = 0;
	rb = gb = bb = 255;
	trf_offset = tgf_offset = tbf_offset = fc_h_offset;
	trb_offset = tgb_offset = tbb_offset = (bc_h_offset + 255) - t_width;
    }    
    else{
      if ((x >= fc_h_offset && x <= fc_h_offset + fc_width) &&
	  (y >= rfc_v_offset && y <= rfc_v_offset + height))
	{
	  r = x - fc_h_offset;
	  trf_offset = x;
	}
      if ((x >= fc_h_offset && x <= fc_h_offset + fc_width) &&
	  (y >= gfc_v_offset && y <= gfc_v_offset + height))
	{
	  g = x - fc_h_offset;
	  tgf_offset = x;
	}
      if ((x >= fc_h_offset && x <= fc_h_offset + fc_width) &&
	  (y >= bfc_v_offset && y <= bfc_v_offset + height))
	{
	  b = x - fc_h_offset;
	  tbf_offset = x;
	}

      if ((x >= bc_h_offset && x <= bc_h_offset + bc_width) &&
	  (y >= rbc_v_offset && y <= rbc_v_offset + height))
	{
	  rb = x - bc_h_offset;
	  trb_offset = x;
	}
      if ((x >= bc_h_offset && x <= bc_h_offset + bc_width) &&
	  (y >= gbc_v_offset && y <= gbc_v_offset + height))
	{
	  gb = x - bc_h_offset;
	  tgb_offset = x;
	}
      if ((x >= bc_h_offset && x <= bc_h_offset + bc_width) &&
	  (y >= bbc_v_offset && y <= bbc_v_offset + height))
	{
	  bb = x - bc_h_offset;
	  tbb_offset = x;
	}
    }
    repaint ();

    return (true);
  }

  public boolean mouseDrag (Event evt, int x, int y)
  {
    if ((x >= fc_h_offset && x <= fc_h_offset + fc_width) &&
	(y >= rfc_v_offset && y <= rfc_v_offset + height))
      {
	r = x - fc_h_offset;
	trf_offset = x;
      }
    if ((x >= fc_h_offset && x <= fc_h_offset + fc_width) &&
	  (y >= gfc_v_offset && y <= gfc_v_offset + height))
      {
	g = x - fc_h_offset;
	tgf_offset = x;
      }
    if ((x >= fc_h_offset && x <= fc_h_offset + fc_width) &&
	(y >= bfc_v_offset && y <= bfc_v_offset + height))
      {
	b = x - fc_h_offset;
	tbf_offset = x;
      }

    if ((x >= bc_h_offset && x <= bc_h_offset + bc_width) &&
	(y >= rbc_v_offset && y <= rbc_v_offset + height))
      {
	rb = x - bc_h_offset;
	trb_offset = x;
      }
    if ((x >= bc_h_offset && x <= bc_h_offset + bc_width) &&
	(y >= gbc_v_offset && y <= gbc_v_offset + height))
      {
	gb = x - bc_h_offset;
	tgb_offset = x;
      }
    if ((x >= bc_h_offset && x <= bc_h_offset + bc_width) &&
	(y >= bbc_v_offset && y <= bbc_v_offset + height))
      {
	bb = x - bc_h_offset;
	tbb_offset = x;
      }
    repaint ();

    return (true);
  }

  public boolean mouseUp (Event evt, int x, int y)
  {
    if(evt.target == cbutton){
      	r = g = b = 0;
	rb = gb = bb = 255;
	trf_offset = tgf_offset = tbf_offset = fc_h_offset;
	trb_offset = tgb_offset = tbb_offset = (bc_h_offset + 255) - t_width;
    }    
    else{
      if ((x >= fc_h_offset && x <= fc_h_offset + fc_width) &&
	(y >= rfc_v_offset && y <= rfc_v_offset + height))
      {
	r = x - fc_h_offset;
	trf_offset = x;
      }
    if ((x >= fc_h_offset && x <= fc_h_offset + fc_width) &&
	  (y >= gfc_v_offset && y <= gfc_v_offset + height))
      {
	g = x - fc_h_offset;
	tgf_offset = x;
      }
    if ((x >= fc_h_offset && x <= fc_h_offset + fc_width) &&
	(y >= bfc_v_offset && y <= bfc_v_offset + height))
      {
	b = x - fc_h_offset;
	tbf_offset = x;
      }

    if ((x >= bc_h_offset && x <= bc_h_offset + bc_width) &&
	(y >= rbc_v_offset && y <= rbc_v_offset + height))
      {
	rb = x - bc_h_offset;
	trb_offset = x;
      }
    if ((x >= bc_h_offset && x <= bc_h_offset + bc_width) &&
	(y >= gbc_v_offset && y <= gbc_v_offset + height))
      {
	gb = x - bc_h_offset;
	tgb_offset = x;
      }
    if ((x >= bc_h_offset && x <= bc_h_offset + bc_width) &&
	(y >= bbc_v_offset && y <= bbc_v_offset + height))
      {
	bb = x - bc_h_offset;
	tbb_offset = x;
      }
    }    

    repaint ();

    return (true);
  }

  public boolean action(Event event,Object arg){
    if(event.target == cbutton){
      	r = g = b = 0;
	rb = gb = bb = 255;
	trf_offset = tgf_offset = tbf_offset = fc_h_offset;
	trb_offset = tgb_offset = tbb_offset = (bc_h_offset + 255) - t_width;
    }
    return true;
  }
  public boolean keyDown (Event evt, int key)
  {
    // now... depending on which key they pressed (R,G or B) the corresponding
    // value will be increased/decreased.
    switch (key)
      {
      case 'r':
	r++;
	if (r > 255)
	  {
	    r = 0;
	    trf_offset -= 255;
	  }
	trf_offset++;
	break;
      case 'R':
	rb--;
	if (rb < 1)
	  {
	    rb = 255;
	    trb_offset += 255;
	  }
	trb_offset--;
	break;
      case 'g':
	g++;
	if (g > 255)
	  {
	    g = 0;
	    tgf_offset -= 255;
	  }
	tgf_offset++;
	break;
      case 'G':
	gb--;
	if (gb < 1)
	  {
	    gb = 255;
	    tgb_offset += 255;
	  }
	tgb_offset--;
	break;
      case 'b':
	b++;
	if (b > 255)
	  {
	    b = 0;
	    tbf_offset -= 255;
	  }
	tbf_offset++;
	break;
      case 'B':
	bb--;
	if (bb < 1)
	  {
	    bb = 255;
	    tbb_offset += 255;
	  }
	tbb_offset--;
	break;
      case 'S':		// reset colors

	r = g = b = 0;
	rb = gb = bb = 255;
	trf_offset = tgf_offset = tbf_offset = fc_h_offset;
	trb_offset = tgb_offset = tbb_offset = (bc_h_offset + 255) - t_width;
      }

    repaint ();
    return true;
  }
}
