#pragma once
class KeyPress
{
public:

    KeyPress(void)
    {
        for (int i = 0; i < 256; i++) {
            m_Keys[i] = false;
        }
    }

    virtual ~KeyPress(void)
    {
    }

    inline void KeyDown(unsigned int index) {
        m_Keys[index] = true;
    }

    inline void KeyUp(unsigned int index) {
        m_Keys[index] = false;
    }

    inline bool IsKeyDown(unsigned int index) {
        return m_Keys[index];
    }

protected:
    bool m_Keys[256];
};

