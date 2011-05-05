//permet de controler l'influence d'un flickeringsquare en fonction de sa distance au point que l'on fixe.

if(m_points.size() >= 1)
			{
/*				flickering square taking each position when asked by pressing P key or O key*/
				if (m_squares.size() >=2 && m_squares[1]->SquareDisplay() && m_squares[0]->SquareDisplay()){
					if (nextPosition){
						if (indicePos<m_positionsTabLeft.size()-1) indicePos++;
						m_squares[1]->SetSquareX(m_positionsTabLeft[indicePos]);
						m_squares[0]->SetSquareX(m_positionsTabRight[indicePos]);
						nextPosition=false;
					}
					if (previousPosition) {
						if (indicePos>0) indicePos--;
						m_squares[1]->SetSquareX(m_positionsTabLeft[indicePos]);
						m_squares[0]->SetSquareX(m_positionsTabRight[indicePos]);
						previousPosition=false;
					}

					if (indicePos==0){
						m_squares[1]->SetSquareX(25);
						m_squares[0]->SetSquareX(m_width - 175);
					}
				}
					app->Draw(*(m_points[0]->GetPoint()));
			}


/*				cursor taking each position when asked by pressing P key or O key*/
/*				if (m_squares.size() !=0 && m_squares[1]->SquareDisplay()){
					if (nextPosition){
						if (indicePos<m_positionsTabLeft.size()-1) indicePos++;
						m_squares[1]->SetSquareX(m_positionsTabLeft[indicePos]);
						nextPosition=false;
					}
					if (previousPosition) {
						if (indicePos>0) indicePos--;
						m_squares[1]->SetSquareX(m_positionsTabLeft[indicePos]);
						previousPosition=false;
					}

					if (indicePos==0){
						m_squares[1]->SetSquareX(25);
					}

					//position[1]=(*(m_points[0]->GetPoint())).GetPointPosition(0).y ;
					position[0]=( (*(m_squares[0]->GetShape())).GetPointPosition(0).x + m_positionsTabLeft[indicePos] + 150) /2 -5 ;
					m_points[0]->SetPosition(position);
					app->Draw(*(m_points[0]->GetPoint()));
				}*/
