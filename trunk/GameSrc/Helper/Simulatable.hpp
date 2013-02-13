#ifndef Simulatable_h__
#define Simulatable_h__

namespace sb
{
	class Simulatable
	{
	private:
		virtual ~Simulatable() {}
		virtual void updateTransform( void ) = 0;
		virtual void smoothenTransform( float remainderRatio ) = 0;
		virtual void resetTransform( void ) = 0;
	};
}

#endif // Simulatable_h__
