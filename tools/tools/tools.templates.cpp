using namespace ldt;

//!Determines if two rectangles overlap.

template<typename T, typename U> 
bool ldt::rects_overlap(T x1, T y1, U w1, U h1, T x2, T y2, U w2, U h2, bool unit_is_collision)
{
        bool in_x;
        bool in_y;

	T x2_w2=(x2+w2);
	T x1_w1=(x1+w1);
	T y2_h2=(y2+h2);
	T y1_h1=(y1+h1);

        //Check if not colliding and negate...
	/* Proof...

0	     10           
|---- A ------|---- B ----|
	     10          20

		If grazing is collision:
			in_x=!( (x2_w2 < x1) || (x2 > x1_w1) );
			in_x=!( (20 < 0) || (10 > 10) );
			in_x=!( false || false );
			in_x=!(false)
			in_x=true

		If not...

			in_x=!( (x2_w2 <= x1) || (x2 >= x1_w1) );
			in_x=!( (20 <= 0) || (10 >= 10) );
			in_x=!( false || true );
			in_x=!(true)
			in_x=false
	*/
	
        if(unit_is_collision)
        {
		in_x=!( (x2_w2 < x1) || (x2 > x1_w1) );
		in_y=!( (y2_h2 < y1) || (y2 > y1_h1) );
        }
        else
        {
		in_x=!( (x2_w2 <= x1) || (x2 >= x1_w1) );
		in_y=!( (y2_h2 <= y1) || (y2 >= y1_h1) );
        }

        return in_x&&in_y;
}

//!Determines if two rectangles overlap and writes the overlap as a result.

template <typename T, typename U> 
bool ldt::rects_overlap(
	T x1, T y1, U w1, U h1, 
	T x2, T y2, U w2, U h2, 
	T& rx, T& ry, U& rw, U& rh, 
	bool unit_is_collision)
{
	if(!rects_overlap(x1, y1, w1, h1, x2, y2, h2, h2, unit_is_collision))
	{
		return false;
	}
	else
	{
		//This is the common box.

		T x1_w1=x1+w1;
		T x2_w2=x2+w2;
		T y1_h1=y1+h1;
		T y2_h2=y2+h2;

		posicion_dimension_segmentos(x1, x1_w1, x2, x2_w2, rx, rw);
		posicion_dimension_segmentos(y1, y1_h1, y2, y2_h2, ry, rh);

		//Important when small dimensions enter the game...
		if(!unit_is_collision)
		{
			if(rw) rw++;
			if(rh) rh++;
		}		

		return true;
	}
}

//!Obtains the overlap of two segments and writes position and dimension.
template <typename T, typename U> 
void ldt::position_dimension_segments(T aini, T afin, T bini, T bfin, T &resultado_pos, U &resultado_dim)
{
	bool c1=aini < bini;
	bool c2=afin < bfin;
	bool c3=bini < aini;
	bool c4=bfin < afin;
	bool c5=aini == bini;
	bool c6=afin == bfin;
	
	//The same...
	if(c5 && c6)
	{
		position_dimension_segments_complete_solution(bini, bfin, aini, afin, resultado_pos, resultado_dim);
	}
	//A to the left of B
	else if(c1 && c2) 
	{
		position_dimension_segments_partial_solution(aini, afin, bini, bfin, resultado_pos, resultado_dim); //Caso 1
	}
	//B to the left of A
	else if(c3 && c4) 
	{
		position_dimension_segments_partial_solution(bini, bfin, aini, afin, resultado_pos, resultado_dim); //Caso 2
	}
	//A inside B
	else if(!c1 && !c4) 
	{
		position_dimension_segments_complete_solution(aini, afin, bini, bfin, resultado_pos, resultado_dim); //Caso 3
	}
	//B inside A
	else if(!c2 && !c3) 
	{
		position_dimension_segments_complete_solution(bini, bfin, aini, afin, resultado_pos, resultado_dim); //Caso 4
	}
}

//!Partial solution helper.

/*
A segment is partially inside the other:

	|------ SEGMENT A ---------|
			     XXXXXXXX
			     |-------- SEGMENT B ------|
*/

template <typename T, typename U> 
void ldt::position_dimension_segments_partial_solution(T aini, T afin, T bini, T bfin, T &resultado_pos, U &resultado_dim)
{
	resultado_pos=bini;
	resultado_dim=afin-bini;
}

//!Partial solution helper.
/*
A segment is completely inside the other...

		|--- SEGMENT A ----|
	|-------XXXX SEGMENT B XXXXX ---|
*/

template <typename T, typename U> 
void ldt::position_dimension_segments_complete_solution(T aini, T afin, T bini, T bfin, T &resultado_pos, U &resultado_dim)
{
	resultado_pos=aini;
	resultado_dim=afin-aini;
}

//!Determines if a point is inside a box.
template <typename T, typename U> 
bool ldt::point_in_box(T cx, T cy, U cw, U ch, T px, T py)
{
        return  (cx <= px && (T)(cx+cw) >= px)
        &&
        (cy <= py && (T)(cy+ch) >= py);
}

//!Determines if a box contains another box.
template <typename T, typename U> 
bool ldt::box_in_box(T pqx, T pqy, U pqw, U pqh, T grx, T gry, U grw, U grh)
{
	T big_xf=grx+grw,
	small_xf=pqx+pqw,
	big_yf=gry+grh,
	small_yf=pqy+pqh;

	return (grx <= pqx
	&& big_xf >= small_xf
	&& gry <= pqy
	&& big_yf >= small_yf);
}
