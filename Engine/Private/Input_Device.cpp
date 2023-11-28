#include "Input_Device.h"


CInput_Device::CInput_Device()
{
}

HRESULT CInput_Device::Initialize(HINSTANCE hInst, HWND hWnd)
{
    FAILED_CHECK(Ready_InputDev(hInst, hWnd));
       
    ZeroMemory(m_byKeyData, sizeof(m_byKeyData));

    return S_OK;
}

void CInput_Device::Tick(void)
{
    HRESULT hr;

    hr = m_pKeyBoard->GetDeviceState(256, m_byKeyState);

    if (FAILED(hr))
    {

        if(hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
            m_pKeyBoard->Acquire();
    }

    hr = m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

    if (FAILED(hr)) {
        if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
            m_pMouse->Acquire();
        }
    }

}

void CInput_Device::LateTick()
{
    for (int i = 0; i < 256; ++i) //������ �ְ� ���� ������ ���� �� false��?
    {
        if (m_byKeyData[i] && !(m_byKeyState[i] & 0x80))
            m_byKeyData[i] = !m_byKeyData[i];

        if (!m_byKeyData[i] && (m_byKeyState[i] & 0x80))
            m_byKeyData[i] = !m_byKeyData[i];
    }

    for (int i = 0; i < DIM_END; ++i) //������ �ְ� ���� ������ ���� �� false��?
    {
        if (m_bMouseData[i] && !(m_tMouseState.rgbButtons[i] & 0x80))
            m_bMouseData[i] = !m_bMouseData[i];

        if (!m_bMouseData[i] && (m_tMouseState.rgbButtons[i] & 0x80))
            m_bMouseData[i] = !m_bMouseData[i];
    }

}

_bool CInput_Device::Key_Pressing(_ubyte byKeyID)
{
    if (m_byKeyState[byKeyID] & 0x80)
        return true;

    return false;
}

_bool CInput_Device::Key_Down(_ubyte byKeyID)
{
    // �������� ���� ���� ���� ���� ������ ���
    if (!m_byKeyData[byKeyID] && (m_byKeyState[byKeyID] & 0x80))
    {
        m_byKeyData[byKeyID] = !m_byKeyData[byKeyID]; //true
        return true;
    }

    return false;
}

_bool CInput_Device::Key_Up(_ubyte byKeyID)
{
    // �������� ���� ���� �ְ� ���� ������ �ʾ��� ���
    if (m_byKeyData[byKeyID] && !(m_byKeyState[byKeyID] & 0x80))
    {
        m_byKeyData[byKeyID] = !m_byKeyData[byKeyID];
        return true;
    }

    return false;
}

_bool CInput_Device::Mouse_Pressing(MOUSEKEYSTATE eMouse)
{
    if (m_tMouseState.rgbButtons[eMouse] & 0x80)
        return true;

    return false;
}

_bool CInput_Device::Mouse_Down(MOUSEKEYSTATE eMouse)
{
    // �������� ���� ���� ���� ���� ������ ���
    if (!m_bMouseData[eMouse] && (m_tMouseState.rgbButtons[eMouse] & 0x80))
    {
        m_bMouseData[eMouse] = !m_bMouseData[eMouse]; //true
        return true;
    }
    return false;
}

_bool CInput_Device::Mouse_Up(MOUSEKEYSTATE eMouse)
{
    // �������� ���� ���� �ְ� ���� ������ �ʾ��� ���
    if (m_bMouseData[eMouse] && !(m_tMouseState.rgbButtons[eMouse] & 0x80))
    {
        m_bMouseData[eMouse] = !m_bMouseData[eMouse];
        return true;
    }

    return false;
}

HRESULT Engine::CInput_Device::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{

    // DInput �İ�ü�� �����ϴ� �Լ�
    if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
        return E_FAIL;

    // Ű���� ��ü ����
    if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
        return E_FAIL;

    // ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
    m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

    // ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
    m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

    // ��ġ�� ���� access ������ �޾ƿ��� �Լ�
    m_pKeyBoard->Acquire();


    // ���콺 ��ü ����
    if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
        return E_FAIL;

    // ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
    m_pMouse->SetDataFormat(&c_dfDIMouse);

    // ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
    m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

    // ��ġ�� ���� access ������ �޾ƿ��� �Լ�
    m_pMouse->Acquire();

    return S_OK;
}


CInput_Device* CInput_Device::Create(HINSTANCE hInst, HWND hWnd)
{
    CInput_Device* pInstance = new CInput_Device();

    if (FAILED(pInstance->Initialize(hInst, hWnd)))
    {
        MSG_BOX("Failed to Created : CInput_Device");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Engine::CInput_Device::Free(void)
{
    Safe_Release(m_pKeyBoard);
    Safe_Release(m_pMouse);
    Safe_Release(m_pInput);
}
;