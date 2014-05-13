#include "pch.h"
#include "FiveteenComponent.h"
#include "Direct3DContentProvider.h"
#include <mutex>

using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Microsoft::WRL;
using namespace Windows::Phone::Graphics::Interop;
using namespace Windows::Phone::Input::Interop;

double g_fps_measure;
bool g_toggle_line;
bool g_toogle_graphics;
std::string g_rw_local_folder;
std::string g_ro_local_folder;
std::mutex g_tmutex;

std::vector< SINPUT > m_Inputs;

namespace PhoneDirect3DXamlAppComponent
{

Direct3DBackground::Direct3DBackground() :
	m_timer(ref new BasicTimer()),
	m_AppState( State::INIT )
{
	Game = new XDRGame();
	m_TouchState = -1;
	m_TouchID    = -1;
}

IDrawingSurfaceBackgroundContentProvider^ Direct3DBackground::CreateContentProvider()
{
	ComPtr<Direct3DContentProvider> provider = Make<Direct3DContentProvider>(this);
	return reinterpret_cast<IDrawingSurfaceBackgroundContentProvider^>(provider.Detach());
}

// IDrawingSurfaceManipulationHandler
void Direct3DBackground::SetManipulationHost(DrawingSurfaceManipulationHost^ manipulationHost)
{
	manipulationHost->PointerPressed +=
		ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DBackground::OnPointerPressed);

	manipulationHost->PointerMoved +=
		ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DBackground::OnPointerMoved);

	manipulationHost->PointerReleased +=
		ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DBackground::OnPointerReleased);
}

// Event Handlers
void Direct3DBackground::OnPointerPressed(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
	g_tmutex.lock();
	if( m_TouchState == -1 &&  m_TouchID ==-1 )
	{
		m_TouchState = 1;
		m_TouchID    = args->CurrentPoint->PointerId;
		// Insert your code here.
		SINPUT input;
		input.pposx =   args->CurrentPoint->Position.X;
		input.pposy =   args->CurrentPoint->Position.Y;
		input.pflag =   FINPUT::P_PRESSED;
		m_Inputs.push_back( input );
		//Game->OnHandleInput(input);
	}
	g_tmutex.unlock();
}

void Direct3DBackground::OnPointerMoved(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
	g_tmutex.lock();
	if( m_TouchState == 1 && m_TouchID == args->CurrentPoint->PointerId )
	{
		// Insert your code here.
		SINPUT input;
		input.pposx =   args->CurrentPoint->Position.X;
		input.pposy =   args->CurrentPoint->Position.Y;
		input.pflag =   FINPUT::P_MOVE;
		m_Inputs.push_back( input );
		//Game->OnHandleInput(input);
	}
	g_tmutex.unlock();
}

void Direct3DBackground::OnPointerReleased(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
{
	g_tmutex.lock();
	if( m_TouchState == 1 && m_TouchID == args->CurrentPoint->PointerId )
	{
		m_TouchState =-1;
		m_TouchID    =-1;
		
		// Insert your code here.
		SINPUT input;
		input.pposx =   args->CurrentPoint->Position.X;
		input.pposy =   args->CurrentPoint->Position.Y;
		input.pflag =   FINPUT::P_RELEASED;
		m_Inputs.push_back( input );
		// Game->OnHandleInput(input);
	}
	g_tmutex.unlock();
}

// Interface With Direct3DContentProvider
HRESULT Direct3DBackground::Connect(_In_ IDrawingSurfaceRuntimeHostNative* host, _In_ ID3D11Device1* device)
{
	//m_renderer = ref new CubeRenderer();
	//m_renderer->Initialize(device);
	//m_renderer->UpdateForWindowSizeChange(WindowBounds.Width, WindowBounds.Height);

	// ro
    auto current = Windows::ApplicationModel::Package::Current;
    auto folder  = current->InstalledLocation;
    auto path    = folder->Path;	
	path        +="\\data\\";
    std::string path2(path->Begin(), path->End());
    g_ro_local_folder = path2;

	// rw
	Platform::String^ str = Windows::Storage::ApplicationData::Current->LocalFolder->Path;
	std::string path3(str->Begin(), str->End());
    g_rw_local_folder =  path3;

	// Restart timer after renderer has finished initializing.
	m_timer->Reset();

	return S_OK;
}

void Direct3DBackground::Disconnect()
{
	//m_renderer = nullptr;
	Game->OnSuspending();
	m_AppState = State::RESUMING;
}

HRESULT Direct3DBackground::PrepareResources(_In_ const LARGE_INTEGER* presentTargetTime, _Inout_ DrawingSurfaceSizeF* desiredRenderTargetSize)
{
	m_timer->Update();
	//m_renderer->Update(m_timer->Total, m_timer->Delta);

	desiredRenderTargetSize->width = RenderResolution.Width;
	desiredRenderTargetSize->height = RenderResolution.Height;

	return S_OK;
}

HRESULT Direct3DBackground::Draw(_In_ ID3D11Device1* device, _In_ ID3D11DeviceContext1* context, _In_ ID3D11RenderTargetView* renderTargetView)
{
	//m_renderer->UpdateDevice(device, context, renderTargetView);
	//m_renderer->Render();
	g_tmutex.lock();
		if( !m_Inputs.empty() )
		{
			Game->OnHandleInput( m_Inputs[0] );
			m_Inputs.erase( m_Inputs.begin() );
		}
	g_tmutex.unlock();
	switch( m_AppState ) 
	{
		case State::INIT :
			Game->Initialize( device, context  );
			m_AppState = State::RUNNING;
			break;
		case State::RUNNING :
			Game->Update( m_timer->Delta, device, context,renderTargetView  );
			break;
		case State::RESUMING :
			Game->Initialize( device, context  );
			Game->OnResuming();
			m_AppState = State::RUNNING;
			break;
		case State::PAUSE :
			break;
		default:
			break;
	};
	RequestAdditionalFrame();

	return S_OK;
}

}