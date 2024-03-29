class SRV_Util
{	
	static bool IsInsideWidget(Widget w, int posx, int posy)
	{
		float w_x, w_y, w_width, w_height;
		w.GetScreenPos(w_x, w_y);
		w.GetScreenSize(w_width, w_height);
		
		int x1, x2, y1, y2;
		x1 = w_x;
		x2 = w_x + w_width;
		y1 = w_y;
		y2 = w_y + w_height;
		
		if (posx > x1 && posx < x2 && posy > y1 && posy < y2)
			return true;
		
		return false;
	}
}