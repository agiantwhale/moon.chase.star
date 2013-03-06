#include <assert.h>
#include <sfTheora.h>
#include "../State/IntroState.hpp"
#include "../Base/Globals.hpp"
#include "../App/Game.hpp"
#include "../GUI/GUIManager.hpp"
#include "../GUI/License.hpp"

#include <TurboActivate.h>

// Support Unicode compilation and non-Windows compilation
#ifdef _WIN32
#include <tchar.h>
#else
#define _T(x) x
typedef char    TCHAR;
#endif

// TODO: paste your Version GUID here.
#define TA_GUID _T("386aeed55136b54c8a3549.62117259")

namespace sb
{
	IntroState::IntroState()
		:	m_introVideo()
	{
	}

	IntroState::~IntroState()
	{
	}

	void IntroState::enter(void)
	{
		GameState::enter();

		Game::getInstance()->setNextStateType(State_MainMenu);

		m_introVideo.load("Resource/Videos/Intro.ogv");
		if( m_introVideo.hasError() )
		{
			TRI_LOG_STR("Unable to load intro video file!");
		}

		m_introVideo.play();
	}

	bool IntroState::update( sf::Time deltaTime )
	{
		m_introVideo.update(deltaTime);

		return m_introVideo.isDone();
	}

	void IntroState::render( void )
	{
		Game::getInstance()->draw(m_introVideo);
	}

	void IntroState::exit(void)
	{
		GameState::exit();

		m_introVideo.unload();

		HRESULT hr;
		GENUINE_OPTIONS opts;
		opts.nLength = sizeof(GENUINE_OPTIONS);

		// How often to verify with the LimeLM servers (90 days)
		opts.nDaysBetweenChecks = 90;

		// The grace period if TurboActivate couldn't connect to the servers.
		// after the grace period is over IsGenuinEx() will return TA_FAIL instead of
		// TA_E_INET or TA_E_INET_DELAYED
		opts.nGraceDaysOnInetErr = 14;

		// In this example we won't show an error if the activation
		// was done offline by passing the TA_SKIP_OFFLINE flag
		opts.flags = TA_SKIP_OFFLINE;

		hr = IsGenuineEx(TA_GUID, &opts);

		if (hr == TA_OK || hr == TA_E_FEATURES_CHANGED || hr == TA_E_INET || hr == TA_E_INET_DELAYED)
		{
			if (hr == TA_E_INET || hr == TA_E_INET_DELAYED)
			{
				ConnectToInternetControl* control = new ConnectToInternetControl(GUIManager::getInstance()->getCanvas());
				control->MakeModal();
			}
		}
		else
		{
			ActivateControl* control = new ActivateControl(GUIManager::getInstance()->getCanvas());
			control->MakeModal();
		}
	}
}