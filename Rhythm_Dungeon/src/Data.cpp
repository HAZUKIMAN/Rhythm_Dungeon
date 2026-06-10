#include "Data.h"


Data* Data::m_instance = nullptr;

Data::Data()
{
	m_selectStage = 0;
}

Data::~Data()
{

}

//---------------------------------
// ŽŸ‚ÌƒXƒe[ƒW‚Ö
//---------------------------------
void Data::NextStage()
{
	m_selectStage++;

	//---------------------------------
	// Å‘å’´‚¦–hŽ~
	//---------------------------------
	if (m_selectStage >= 9)
	{
		m_selectStage = 8;
	}

}

int Data::GetStageCount()
{
	return 9;
}