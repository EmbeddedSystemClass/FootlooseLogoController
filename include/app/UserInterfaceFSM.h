//
// ex: set ro:
// DO NOT EDIT.
// generated by smc (http://smc.sourceforge.net/)
// from file : UserInterface.sm
//

#ifndef USERINTERFACEFSM_H
#define USERINTERFACEFSM_H


#define SMC_NO_EXCEPTIONS

#include <statemap.h>

// Forward declarations.
class UIMap;
class UIMap_Reset;
class UIMap_UITest;
class UIMap_AddressDisplay;
class UIMap_DisplayOff;
class UIMap_TestDisplay;
class UIMap_MenuMode;
class UIMap_MenuDisplay;
class UIMap_MenuExit;
class UIMap_MenuModeDmx;
class UIMap_MenuModeRainBow;
class UIMap_MenuModeTest;
class UIMap_MenuDisplayShow;
class UIMap_MenuDisplayIncrease;
class UIMap_Default;
class UserInterfaceState;
class UserInterfaceFSM;
class UserInterface;

class UserInterfaceState :
    public statemap::State
{
public:

    UserInterfaceState(const char * const name, const int stateId)
    : statemap::State(name, stateId)
    {};

    virtual void Entry(UserInterfaceFSM&) {};
    virtual void Exit(UserInterfaceFSM&) {};

    virtual void BtnModePressed(UserInterfaceFSM& context);
    virtual void BtnOkPressed(UserInterfaceFSM& context);
    virtual void DmxAddressChanged(UserInterfaceFSM& context);
    virtual void Ready(UserInterfaceFSM& context);
    virtual void SwModeDisabled(UserInterfaceFSM& context);
    virtual void SwModeEnabled(UserInterfaceFSM& context);
    virtual void Tick(UserInterfaceFSM& context);
    virtual void displaySleepTimerElapsed(UserInterfaceFSM& context);
    virtual void updateTimerElapsed(UserInterfaceFSM& context);

protected:

    virtual void Default(UserInterfaceFSM& context);
};

class UIMap
{
public:

    static UIMap_Reset Reset;
    static UIMap_UITest UITest;
    static UIMap_AddressDisplay AddressDisplay;
    static UIMap_DisplayOff DisplayOff;
    static UIMap_TestDisplay TestDisplay;
    static UIMap_MenuMode MenuMode;
    static UIMap_MenuDisplay MenuDisplay;
    static UIMap_MenuExit MenuExit;
    static UIMap_MenuModeDmx MenuModeDmx;
    static UIMap_MenuModeRainBow MenuModeRainBow;
    static UIMap_MenuModeTest MenuModeTest;
    static UIMap_MenuDisplayShow MenuDisplayShow;
    static UIMap_MenuDisplayIncrease MenuDisplayIncrease;
};

class UIMap_Default :
    public UserInterfaceState
{
public:

    UIMap_Default(const char * const name, const int stateId)
    : UserInterfaceState(name, stateId)
    {};

    virtual void Ready(UserInterfaceFSM& context);
    virtual void Tick(UserInterfaceFSM& context);
    virtual void Default(UserInterfaceFSM& context);
    virtual void SwModeEnabled(UserInterfaceFSM& context);
    virtual void DmxAddressChanged(UserInterfaceFSM& context);
};

class UIMap_Reset :
    public UIMap_Default
{
public:
    UIMap_Reset(const char * const name, const int stateId)
    : UIMap_Default(name, stateId)
    {};

    virtual void DmxAddressChanged(UserInterfaceFSM& context);
    virtual void Ready(UserInterfaceFSM& context);
};

class UIMap_UITest :
    public UIMap_Default
{
public:
    UIMap_UITest(const char * const name, const int stateId)
    : UIMap_Default(name, stateId)
    {};

    virtual void Entry(UserInterfaceFSM&);
    virtual void Exit(UserInterfaceFSM&);
    virtual void DmxAddressChanged(UserInterfaceFSM& context);
    virtual void updateTimerElapsed(UserInterfaceFSM& context);
};

class UIMap_AddressDisplay :
    public UIMap_Default
{
public:
    UIMap_AddressDisplay(const char * const name, const int stateId)
    : UIMap_Default(name, stateId)
    {};

    virtual void Entry(UserInterfaceFSM&);
    virtual void Exit(UserInterfaceFSM&);
    virtual void BtnModePressed(UserInterfaceFSM& context);
    virtual void DmxAddressChanged(UserInterfaceFSM& context);
    virtual void displaySleepTimerElapsed(UserInterfaceFSM& context);
    virtual void updateTimerElapsed(UserInterfaceFSM& context);
};

class UIMap_DisplayOff :
    public UIMap_Default
{
public:
    UIMap_DisplayOff(const char * const name, const int stateId)
    : UIMap_Default(name, stateId)
    {};

    virtual void Entry(UserInterfaceFSM&);
    virtual void BtnModePressed(UserInterfaceFSM& context);
    virtual void BtnOkPressed(UserInterfaceFSM& context);
};

class UIMap_TestDisplay :
    public UIMap_Default
{
public:
    UIMap_TestDisplay(const char * const name, const int stateId)
    : UIMap_Default(name, stateId)
    {};

    virtual void Entry(UserInterfaceFSM&);
    virtual void Exit(UserInterfaceFSM&);
    virtual void SwModeDisabled(UserInterfaceFSM& context);
};

class UIMap_MenuMode :
    public UIMap_Default
{
public:
    UIMap_MenuMode(const char * const name, const int stateId)
    : UIMap_Default(name, stateId)
    {};

    virtual void Entry(UserInterfaceFSM&);
    virtual void BtnModePressed(UserInterfaceFSM& context);
    virtual void BtnOkPressed(UserInterfaceFSM& context);
};

class UIMap_MenuDisplay :
    public UIMap_Default
{
public:
    UIMap_MenuDisplay(const char * const name, const int stateId)
    : UIMap_Default(name, stateId)
    {};

    virtual void Entry(UserInterfaceFSM&);
    virtual void BtnModePressed(UserInterfaceFSM& context);
    virtual void BtnOkPressed(UserInterfaceFSM& context);
};

class UIMap_MenuExit :
    public UIMap_Default
{
public:
    UIMap_MenuExit(const char * const name, const int stateId)
    : UIMap_Default(name, stateId)
    {};

    virtual void Entry(UserInterfaceFSM&);
    virtual void BtnModePressed(UserInterfaceFSM& context);
    virtual void BtnOkPressed(UserInterfaceFSM& context);
};

class UIMap_MenuModeDmx :
    public UIMap_Default
{
public:
    UIMap_MenuModeDmx(const char * const name, const int stateId)
    : UIMap_Default(name, stateId)
    {};

    virtual void Entry(UserInterfaceFSM&);
    virtual void BtnModePressed(UserInterfaceFSM& context);
    virtual void BtnOkPressed(UserInterfaceFSM& context);
};

class UIMap_MenuModeRainBow :
    public UIMap_Default
{
public:
    UIMap_MenuModeRainBow(const char * const name, const int stateId)
    : UIMap_Default(name, stateId)
    {};

    virtual void Entry(UserInterfaceFSM&);
    virtual void BtnModePressed(UserInterfaceFSM& context);
    virtual void BtnOkPressed(UserInterfaceFSM& context);
};

class UIMap_MenuModeTest :
    public UIMap_Default
{
public:
    UIMap_MenuModeTest(const char * const name, const int stateId)
    : UIMap_Default(name, stateId)
    {};

    virtual void Entry(UserInterfaceFSM&);
    virtual void BtnModePressed(UserInterfaceFSM& context);
    virtual void BtnOkPressed(UserInterfaceFSM& context);
};

class UIMap_MenuDisplayShow :
    public UIMap_Default
{
public:
    UIMap_MenuDisplayShow(const char * const name, const int stateId)
    : UIMap_Default(name, stateId)
    {};

    virtual void Entry(UserInterfaceFSM&);
    virtual void BtnModePressed(UserInterfaceFSM& context);
    virtual void BtnOkPressed(UserInterfaceFSM& context);
};

class UIMap_MenuDisplayIncrease :
    public UIMap_Default
{
public:
    UIMap_MenuDisplayIncrease(const char * const name, const int stateId)
    : UIMap_Default(name, stateId)
    {};

    virtual void Entry(UserInterfaceFSM&);
    virtual void Tick(UserInterfaceFSM& context);
};

class UserInterfaceFSM :
    public statemap::FSMContext
{
public:

    explicit UserInterfaceFSM(UserInterface& owner)
    : FSMContext(UIMap::Reset),
      _owner(owner)
    {};

    UserInterfaceFSM(UserInterface& owner, const statemap::State& state)
    : FSMContext(state),
      _owner(owner)
    {};

    virtual void enterStartState()
    {
        getState().Entry(*this);
        return;
    }

    inline UserInterface& getOwner()
    {
        return (_owner);
    };

    inline UserInterfaceState& getState()
    {
        assert(_state != NULL);

        return static_cast<UserInterfaceState&>(*_state);
    };

    inline void BtnModePressed()
    {
        getState().BtnModePressed(*this);
    };

    inline void BtnOkPressed()
    {
        getState().BtnOkPressed(*this);
    };

    inline void DmxAddressChanged()
    {
        getState().DmxAddressChanged(*this);
    };

    inline void Ready()
    {
        getState().Ready(*this);
    };

    inline void SwModeDisabled()
    {
        getState().SwModeDisabled(*this);
    };

    inline void SwModeEnabled()
    {
        getState().SwModeEnabled(*this);
    };

    inline void Tick()
    {
        getState().Tick(*this);
    };

    inline void displaySleepTimerElapsed()
    {
        getState().displaySleepTimerElapsed(*this);
    };

    inline void updateTimerElapsed()
    {
        getState().updateTimerElapsed(*this);
    };

private:
    UserInterface& _owner;
};


#endif // USERINTERFACEFSM_H

//
// Local variables:
//  buffer-read-only: t
// End:
//
