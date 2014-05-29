// TestVerletDlg.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include <list>
#include <vector>
#include "TestVerlet.h"
#include "TestVerletDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// If M_PI doesn't exist then define it now
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static void VerletInit( void );
static void VerletTick( void );

/////////////////////////////////////////////////////////////////////////////
// CTestVerletDlg dialog

CTestVerletDlg::CTestVerletDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestVerletDlg::IDD, pParent) , mTimes(0)
{
	//{{AFX_DATA_INIT(CTestVerletDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestVerletDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestVerletDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestVerletDlg, CDialog)
	//{{AFX_MSG_MAP(CTestVerletDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestVerletDlg message handlers

BOOL CTestVerletDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	VerletInit();
//	SetTimer(0,1000,0);	// 1 fps
//	SetTimer(0,500,0);	// 2 fps
//	SetTimer(0,100,0);	// 10 fps
	SetTimer(0,20,0);	// 50 fps
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}


class Vec2f
{
public:
	Vec2f( void );
	Vec2f( const float x , const float y );
	Vec2f( const Vec2f &o );

	virtual ~Vec2f( void );

	float Length( void ) const;
	float LengthSquared( void ) const;
	static float Dot( const Vec2f &a , const Vec2f &b );

	bool operator==( const Vec2f &o) const;
	bool operator!=( const Vec2f &o) const;

	void operator+=( const Vec2f &o );
	void operator-=( const Vec2f &o );
	void operator*=( const Vec2f &o );
	void operator/=( const Vec2f &o );
	Vec2f operator+( const Vec2f &o ) const;
	Vec2f operator-( const Vec2f &o ) const;
	Vec2f operator*( const Vec2f &o ) const;
	Vec2f operator/( const Vec2f &o ) const;

	float mX , mY;
};

Vec2f::Vec2f( void ) : mX( 0.0f ) , mY( 0.0f )
{
}

Vec2f::Vec2f( const float x , const float y ) : mX( x ) , mY( y )
{
}

Vec2f::Vec2f( const Vec2f &o ) : mX( o.mX ) , mY( o.mY )
{
}

Vec2f::~Vec2f( void )
{
}

float Vec2f::Length( void ) const
{
	return sqrt( ( mX * mX ) + ( mY * mY ) );
}

float Vec2f::LengthSquared( void ) const
{
	return ( mX * mX ) + ( mY * mY );
}

float Vec2f::Dot( const Vec2f &a , const Vec2f &b )
{
	return (a.mX * b.mX) + (a.mY * b.mY);
}


bool Vec2f::operator==(const Vec2f &o) const
{
	return ( mX == o.mX ) && ( mY == o.mY );
}
bool Vec2f::operator!=(const Vec2f &o) const
{
	return ( mX != o.mX ) || ( mY != o.mY );
}

void Vec2f::operator+=(const Vec2f &o)
{
	mX += o.mX;
	mY += o.mY;
}
void Vec2f::operator-=(const Vec2f &o)
{
	mX -= o.mX;
	mY -= o.mY;
}
void Vec2f::operator*=(const Vec2f &o)
{
	mX *= o.mX;
	mY *= o.mY;
}
void Vec2f::operator/=(const Vec2f &o)
{
	mX /= o.mX;
	mY /= o.mY;
}

Vec2f Vec2f::operator+(const Vec2f &o) const
{
	return Vec2f( mX + o.mX , mY + o.mY );
}
Vec2f Vec2f::operator-(const Vec2f &o) const
{
	return Vec2f( mX - o.mX , mY - o.mY );
}
Vec2f Vec2f::operator*(const Vec2f &o) const
{
	return Vec2f( mX * o.mX , mY * o.mY );
}
Vec2f Vec2f::operator/(const Vec2f &o) const
{
	return Vec2f( mX / o.mX , mY / o.mY );
}

class Constraint;

class Particle
{
public:
	Particle( void )
	{
	}
	virtual ~Particle ( void )
	{
	}

	void SetPosition( const Vec2f &pos )
	{
		mCurrentPosition = pos;
		mLastPosition = pos;
	}

	Vec2f mCurrentPosition , mLastPosition , mDiff;
	std::list<Constraint*> mConstraints;
};

class Constraint
{
public:
	Constraint( Particle& particle ) : mParticle( particle )
	{
	}

	virtual ~Constraint( void )
	{
	}

	virtual void Solve( void ) = 0;

	virtual void Render( CDC &dcMemory )
	{
	}

	Particle& mParticle;
};

class ConstraintPosition : public Constraint
{
public:
	ConstraintPosition( Particle& particle , const Vec2f& position) : Constraint( particle ) , mPosition( position )
	{
	}

	virtual void Solve( void )
	{
		mParticle.mCurrentPosition = mPosition;
	}

	Vec2f mPosition;
};

class ConstraintLength : public Constraint
{
public:
	ConstraintLength( Particle& particle , Particle& other ) : Constraint( particle ) , mOther( other )
	{
		mLength = ( mParticle.mCurrentPosition - mOther.mCurrentPosition ).Length();
	}

	virtual void Solve( void );

	virtual void Render( CDC &dcMemory )
	{
		dcMemory.MoveTo( (int) mParticle.mCurrentPosition.mX , (int) mParticle.mCurrentPosition.mY );
		dcMemory.LineTo( (int) mOther.mCurrentPosition.mX , (int) mOther.mCurrentPosition.mY );
	}

	float mLength;
	Particle& mOther;
};

void ConstraintLength::Solve( void )
{
	Vec2f diff = mParticle.mCurrentPosition - mOther.mCurrentPosition;
	float d1 = diff.Length();
	if ( d1 != 0.0f )
	{
		// Move the particles by half the distance needed to get back to the constraint
		d1 = ( d1 - mLength ) / ( 2.0f * d1 );

		Vec2f dmove( diff.mX * d1 , diff.mY * d1 );

		mParticle.mCurrentPosition -= dmove;
		mOther.mCurrentPosition += dmove;
	}
}


class ConstraintQuad : public Constraint
{
public:
	ConstraintQuad( Particle& particle , Particle& other1 , Particle& other2 , Particle& other3 , Particle& other4 ) : Constraint( particle ) , mOther1( other1 ) , mOther2( other2 ) , mOther3( other3 ) , mOther4( other4 )
	{
	}

	virtual void Solve( void );

	Particle& mOther1;
	Particle& mOther2;
	Particle& mOther3;
	Particle& mOther4;
};

static Vec2f MoveToLine( const Vec2f &v , const Vec2f &w , const Vec2f &p )
{
	Vec2f WmV = w - v;
	const float l2 = WmV.LengthSquared();	// i.e. |w-v|^2 -  avoid a sqrt
	if (l2 == 0.0f)
	{
		return p;
	}

	// Consider the line extending the segment, parameterized as v + t (w - v).
	// We find projection of point p onto the line. 
	// It falls where t = [(p-v) . (w-v)] / |w-v|^2
	const float t = Vec2f::Dot(p - v , WmV) / l2;
	if (t < 0.0f)
	{
		return p;       // Beyond the 'v' end of the segment
	}
	else if (t > 1.0f)
	{
		return p;  // Beyond the 'w' end of the segment
	}

	Vec2f projection( WmV.mX * t , WmV.mY * t );  // Projection falls on the segment
	projection += v;
	return projection;
//	return distance(p, projection);
}

void ConstraintQuad::Solve( void )
{
	Vec2f pt = mParticle.mCurrentPosition;
	Vec2f p1 = mOther1.mCurrentPosition;
	Vec2f p2 = mOther2.mCurrentPosition;
	Vec2f p3 = mOther3.mCurrentPosition;
	Vec2f p4 = mOther4.mCurrentPosition;

	//					a		b		a				a		b	a
	float AB = (pt.mY-p1.mY)*(p2.mX-p1.mX) - (pt.mX-p1.mX)*(p2.mY-p1.mY);
	float BC = (pt.mY-p2.mY)*(p3.mX-p2.mX) - (pt.mX-p2.mX)*(p3.mY-p2.mY);
	float CD = (pt.mY-p3.mY)*(p4.mX-p3.mX) - (pt.mX-p3.mX)*(p4.mY-p3.mY);
	float DA = (pt.mY-p4.mY)*(p1.mX-p4.mX) - (pt.mX-p4.mX)*(p1.mY-p4.mY);

	float e1 , e2 , e3 , e4;
	e1 = AB * BC;
	e2 = BC * CD;
	e3 = CD * DA;
	e4 = DA * AB;
	if ( ( e1 > 0.0f ) && ( e2 > 0.0f ) && ( e3 > 0.0f ) && ( e4 > 0.0f ) )
	{
		// The point is inside the quad, so move it back to the nearest edge.
		Vec2f p;
		if ( ( AB < BC ) && ( AB < CD ) && ( AB < DA ) )
		{
			p = MoveToLine( mOther1.mCurrentPosition , mOther2.mCurrentPosition , mParticle.mCurrentPosition );
//			p = MoveToLine( mOther2.mCurrentPosition , mOther3.mCurrentPosition , p );
		}
		else if ( ( BC < AB ) && ( BC < CD ) && ( BC < DA ) )
		{
			p = MoveToLine( mOther2.mCurrentPosition , mOther3.mCurrentPosition , mParticle.mCurrentPosition );
//			p = MoveToLine( mOther3.mCurrentPosition , mOther4.mCurrentPosition , p );
		}
		else if ( ( CD < AB ) && ( CD < AB ) && ( CD < BC ) )
		{
			p = MoveToLine( mOther3.mCurrentPosition , mOther4.mCurrentPosition , mParticle.mCurrentPosition );
//			p = MoveToLine( mOther4.mCurrentPosition , mOther1.mCurrentPosition , p );
		}
		else
		{
			p = MoveToLine( mOther4.mCurrentPosition , mOther1.mCurrentPosition , mParticle.mCurrentPosition );
//			p = MoveToLine( mOther1.mCurrentPosition , mOther2.mCurrentPosition , p );
		}
		mParticle.mCurrentPosition = p;
	}
}



class ConstraintRadius : public Constraint
{
public:
	ConstraintRadius( Particle& particle1 , Particle& particle2 , Particle& particle3 , Particle& particle4 , Particle& other1 , Particle& other2 , Particle& other3 , Particle& other4 ) : Constraint( particle1 ) , mParticle2( particle2 ) , mParticle3( particle3 ) , mParticle4( particle4 ) , mOther1( other1 ) , mOther2( other2 ) , mOther3( other3 ) , mOther4( other4 )
	{
		mSize = (other1.mCurrentPosition - other2.mCurrentPosition).Length() / 2.0f;
	}

	virtual void Render( CDC &dcMemory )
	{
		Vec2f quad1 = mParticle.mCurrentPosition + mParticle2.mCurrentPosition + mParticle3.mCurrentPosition + mParticle4.mCurrentPosition;
		quad1.mX /= 4.0f;
		quad1.mY /= 4.0f;
		dcMemory.Ellipse( (int) (quad1.mX - mSize) , (int) (quad1.mY  - mSize) , (int) (quad1.mX + mSize) , (int) (quad1.mY + mSize) );

		Vec2f quad2 = mOther1.mCurrentPosition + mOther2.mCurrentPosition + mOther3.mCurrentPosition + mOther4.mCurrentPosition;
		quad2.mX /= 4.0f;
		quad2.mY /= 4.0f;
		dcMemory.Ellipse( (int) (quad2.mX - mSize) , (int) (quad2.mY  - mSize) , (int) (quad2.mX + mSize) , (int) (quad2.mY + mSize) );
	}

	virtual void Solve( void );

	Particle& mParticle2;
	Particle& mParticle3;
	Particle& mParticle4;
	Particle& mOther1;
	Particle& mOther2;
	Particle& mOther3;
	Particle& mOther4;

	float mSize;
};


void ConstraintRadius::Solve( void )
{
	// Find the middle for each one
	Vec2f quad1 = mParticle.mCurrentPosition + mParticle2.mCurrentPosition + mParticle3.mCurrentPosition + mParticle4.mCurrentPosition;
	quad1.mX /= 4.0f;
	quad1.mY /= 4.0f;
	Vec2f quad2 = mOther1.mCurrentPosition + mOther2.mCurrentPosition + mOther3.mCurrentPosition + mOther4.mCurrentPosition;
	quad2.mX /= 4.0f;
	quad2.mY /= 4.0f;

	Vec2f diff = quad1 - quad2;
	float distance = diff.Length();

	float theSize = mSize * 2.0f;
	// If the quads intersect then move it away by a factor of the distance and vector between the two quads
	if ( distance < theSize )
	{
		distance = theSize - distance;
		diff.mX *= distance;
		diff.mY *= distance;
		diff.mX /= (theSize * 2.0f);
		diff.mY /= (theSize * 2.0f);

		mParticle.mCurrentPosition += diff;
		mParticle2.mCurrentPosition += diff;
		mParticle3.mCurrentPosition += diff;
		mParticle4.mCurrentPosition += diff;

		mOther1.mCurrentPosition -= diff;
		mOther2.mCurrentPosition -= diff;
		mOther3.mCurrentPosition -= diff;
		mOther4.mCurrentPosition -= diff;
	}
}


std::vector<Particle> sParticles;

static void DoRounding( void )
{
#if 1
	// Simulate numerical accuracy that we will need for the C64. After most calculations this is run to make sure all values use limited bits.
//	const float accuracy = 32.0f;
	const float accuracy = 65536.0f;

	size_t i;
	for ( i = 0 ; i < sParticles.size() ; i++ )
	{
		sParticles[i].mCurrentPosition.mX = float(((int)(sParticles[i].mCurrentPosition.mX * accuracy))) / accuracy;
		sParticles[i].mCurrentPosition.mY = float(((int)(sParticles[i].mCurrentPosition.mY * accuracy))) / accuracy;

		sParticles[i].mLastPosition.mX = float(((int)(sParticles[i].mLastPosition.mX * accuracy))) / accuracy;
		sParticles[i].mLastPosition.mY = float(((int)(sParticles[i].mLastPosition.mY * accuracy))) / accuracy;
	}
#endif
}

static void VerletInit( void )
{
#if 1
	// Setup eight quads that will fall and collide.
	sParticles.resize(32);
//	sParticles.resize(8);
	Vec2f o;
	o = Vec2f( 165 , 0 );
	// The size is 28 because using x/y expanded sprites this means 14 pixels. 14 * sqrt(2) = 19.79 which means a rotated square will fit inside a single sprite.

	sParticles[0].SetPosition( o + Vec2f( 0 , 0 ) );
	sParticles[1].SetPosition( o + Vec2f( 28 , 0 ) );
	sParticles[2].SetPosition( o + Vec2f( 28 , 28 ) );
	sParticles[3].SetPosition( o + Vec2f( 0 , 28 ) );

	o = Vec2f( 135 , 0 );
	sParticles[4].SetPosition( o + Vec2f( 0 , 0 ) );
	sParticles[5].SetPosition( o + Vec2f( 28 , 0 ) );
	sParticles[6].SetPosition( o + Vec2f( 28 , 28 ) );
	sParticles[7].SetPosition( o + Vec2f( 0 , 28 ) );

	o = Vec2f( 105 , 0 );
	sParticles[8].SetPosition( o + Vec2f( 0 , 0 ) );
	sParticles[9].SetPosition( o + Vec2f( 28 , 0 ) );
	sParticles[10].SetPosition( o + Vec2f( 28 , 28 ) );
	sParticles[11].SetPosition( o + Vec2f( 0 , 28 ) );

	o = Vec2f( 160 , -30 );
	sParticles[12].SetPosition( o + Vec2f( 0 , 0 ) );
	sParticles[13].SetPosition( o + Vec2f( 28 , 0 ) );
	sParticles[14].SetPosition( o + Vec2f( 28 , 28 ) );
	sParticles[15].SetPosition( o + Vec2f( 0 , 28 ) );

	o = Vec2f( 130 , 80 );
	sParticles[16].SetPosition( o + Vec2f( 0 , 0 ) );
	sParticles[17].SetPosition( o + Vec2f( 28 , 0 ) );
	sParticles[18].SetPosition( o + Vec2f( 28 , 28 ) );
	sParticles[19].SetPosition( o + Vec2f( 0 , 28 ) );

	o = Vec2f( 100 , 90 );
	sParticles[20].SetPosition( o + Vec2f( 0 , 0 ) );
	sParticles[21].SetPosition( o + Vec2f( 28 , 0 ) );
	sParticles[22].SetPosition( o + Vec2f( 28 , 28 ) );
	sParticles[23].SetPosition( o + Vec2f( 0 , 28 ) );

	o = Vec2f( 160 , 170 );
	sParticles[24].SetPosition( o + Vec2f( 0 , 0 ) );
	sParticles[25].SetPosition( o + Vec2f( 28 , 0 ) );
	sParticles[26].SetPosition( o + Vec2f( 28 , 28 ) );
	sParticles[27].SetPosition( o + Vec2f( 0 , 28 ) );

	o = Vec2f( 130 , 170 );
	sParticles[28].SetPosition( o + Vec2f( 0 , 0 ) );
	sParticles[29].SetPosition( o + Vec2f( 28 , 0 ) );
	sParticles[30].SetPosition( o + Vec2f( 28 , 28 ) );
	sParticles[31].SetPosition( o + Vec2f( 0 , 28 ) );

	// Setup constraints, taking into account the initial position of the particles
	size_t i , j , k;
	for ( k = 0 ; k < sParticles.size() ; k += 4 )
	{
		for ( i = 0 ; i < 4 ; i++ )
		{
			j = ( i + 1 ) & 3;
			sParticles[ k + i ].mConstraints.push_back( new ConstraintLength( sParticles[ k + i ] , sParticles[ k + j ] ) );

			j = ( i + 2 ) & 3;
			sParticles[ k + i ].mConstraints.push_back( new ConstraintLength( sParticles[ k + i ] , sParticles[ k + j ] ) );

			j = ( i + 3 ) & 3;
			sParticles[ k + i ].mConstraints.push_back( new ConstraintLength( sParticles[ k + i ] , sParticles[ k + j ] ) );
		}
	}

	for ( i = 0 ; i < sParticles.size() ; i += 4 )
//	for ( i = 0 ; i < 4 ; i += 4 )
	{
		// Now the quads
		for ( k = 0 ; k < sParticles.size() ; k += 4 )
		{
			if ( k != i )
			{
				sParticles[ i ].mConstraints.push_back( new ConstraintRadius( sParticles[ i ] , sParticles[ i + 1 ] , sParticles[ i + 2 ] , sParticles[ i + 3 ] , sParticles[ k + 0 ] , sParticles[ k + 1 ] , sParticles[ k + 2 ] , sParticles[ k + 3 ] ) );

				for ( j = 0 ; j < 4 ; j++ )
				{
					sParticles[ i + j ].mConstraints.push_back( new ConstraintQuad( sParticles[ i + j ] , sParticles[ k + 0 ] , sParticles[ k + 1 ] , sParticles[ k + 2 ] , sParticles[ k + 3 ] ) );
				}
			}
		}
	}

#else
	// A simple swinging rope
	sParticles.resize(9);
	Vec2f o( 100,0 );
	size_t i;
	for ( i = 0 ; i < sParticles.size() ; i++ )
	{
		sParticles[ i ].SetPosition( o + Vec2f( (float) i * 20 , 0 ) );
	}

	sParticles[ 0 ].mConstraints.push_back( new ConstraintPosition( sParticles[ 0 ] , sParticles[ 0 ].mCurrentPosition ) );
	for ( i = 1 ; i < sParticles.size() ; i++ )
	{
		sParticles[ i ].mConstraints.push_back( new ConstraintLength( sParticles[ i ] , sParticles[ i - 1 ] ) );
	}
#endif
}

static void VerletTick( void )
{
	DoRounding();

	Vec2f force( 0.0f , 0.25f );	// Gravity
//	Vec2f force( 0.0f , 1.0f );	// Gravity
//	force += Vec2f( 1.0f , 0.0f );	// Wind, blowing to the right

	size_t i;
	// Choose what to the force on
	for (i = 0 ; i < sParticles.size() ; i++ )
//	for (i = 0 ; i < 4 ; i++ )
	{
		Vec2f temp = sParticles[i].mCurrentPosition;

		// No frame rate compensation, assume the force is constant per frame no matter what
		Vec2f diff = sParticles[i].mCurrentPosition - sParticles[i].mLastPosition;
		sParticles[i].mDiff = diff;
		
		// Adds the difference from the last frame plus force to the current position
		diff += force;
		sParticles[i].mCurrentPosition += diff;

		sParticles[i].mLastPosition = temp;
	}

	DoRounding();

	// Satisfy constraints for each particle
	int p;
	// The more iterations, the more stable the simulation, in general.
	for ( p = 1 ; p <= 10 ; p++ )	// The number of iterations to run the constraint pass
//	for ( p = 1 ; p <= 3 ; p++ )	// The number of iterations to run the constraint pass
	{
#if 0
		// Pin one of the corners as it is falling, checks the simulation can cope with changes in numbers
		if ( sParticles[ 6 ].mCurrentPosition.mY > 125 )
		{
			sParticles[ 6 ].mCurrentPosition.mY = 125;
		}
#endif

		for (i = 0 ; i < sParticles.size() ; i++ )
		{
			std::list<Constraint*>::iterator j;
			for ( j = sParticles[ i ].mConstraints.begin() ; j != sParticles[ i ].mConstraints.end() ; j++ )
			{
				(*j)->Solve();

				DoRounding();
			}

			// Apply other constraints, for the screen bounds
			if ( sParticles[ i ].mCurrentPosition.mX < 0 )
			{
				sParticles[ i ].mCurrentPosition.mX = 0;
			}

#if 0
			if ( sParticles[ i ].mCurrentPosition.mY < 0 )
			{
				sParticles[ i ].mCurrentPosition.mY = 0;
			}
#endif

			if ( sParticles[ i ].mCurrentPosition.mX > 319 )
			{
				sParticles[ i ].mCurrentPosition.mX = 319;
			}

			if ( sParticles[ i ].mCurrentPosition.mY > 200 )
			{
				sParticles[ i ].mCurrentPosition.mY = 200;
#if 0
				if ( sParticles[ i ].mCurrentPosition.mY > sParticles[ i ].mLastPosition.mY )
				{
					sParticles[ i ].mCurrentPosition.mY -= sParticles[ i ].mCurrentPosition.mY - sParticles[ i ].mLastPosition.mY;
				}
#endif
			}

			DoRounding();
		}
	}
}


void CTestVerletDlg::OnPaint() 
{
	CPaintDC theDC(this); // device context for painting

	CRect rect( 0 , 0 , 320 , 200 );
//	GetClientRect(&rect);

	CBitmap bmp;
	// Creating a bitmap of 320x200 is useful if we want to output frames suitable for a C64 animation later on
	bmp.CreateBitmap(rect.right,rect.bottom,1,32,0);

	CDC dcMemory;
	dcMemory.CreateCompatibleDC(&theDC);
	CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);

//	dcMemory.FillSolidRect(0,0,rect.right,rect.bottom,RGB(0,0,0));

	// Draw constraints into the C64 resolution screen
	CPen newPen(PS_SOLID,1,RGB(0,255,0));
	CPen *pOldPen = dcMemory.SelectObject(&newPen);
	size_t i;
	for ( i = 0 ; i < sParticles.size() ; i++ )
	{
		std::list<Constraint*>::iterator j;
		for ( j = sParticles[ i ].mConstraints.begin() ; j != sParticles[ i ].mConstraints.end() ; j++ )
		{
			(*j)->Render( dcMemory );
		}
	}
	dcMemory.SelectObject(pOldPen);

	// Now blit the in memory DC to the window DC
	CRect rect2;
	GetClientRect(&rect2);
//	theDC.BitBlt(0, 0, rect.right, rect.bottom, &dcMemory, 0, 0, SRCCOPY);
	theDC.StretchBlt(0, 0, rect2.right, rect2.bottom, &dcMemory, 0, 0, rect.right, rect.bottom , SRCCOPY);

	dcMemory.SelectObject(pOldBitmap);

	// After all the painting is done we can draw debug text with this...
	theDC.SetTextColor(RGB(255,255,255));
	theDC.SetBkColor(RGB(0,0,0));
	theDC.SetBkMode(TRANSPARENT);
	theDC.TextOut(10,10,"Hello");
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestVerletDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CTestVerletDlg::OnEraseBkgnd(CDC* pDC) 
{
//	return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void CTestVerletDlg::OnTimer(UINT nIDEvent) 
{
	// Animate
	mTimes++;
	VerletTick();
	// Draw the window
	RedrawWindow();	
	CDialog::OnTimer(nIDEvent);
}
