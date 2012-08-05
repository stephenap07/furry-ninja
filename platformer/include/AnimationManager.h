#ifndef ANIMATION_H_
#define ANIMATION_H_

// Animation Typedefs 
typedef sf::IntRect frame_t; 
typedef std::vector<frame_t> animation_t; ; 

struct Animation { 
  Animation(const animation_t & a, float t = 0.05f, int cur_frame=0) 
  : _anim(a), _time_limit(t), _current_frame(cur_frame), _current_time(0.0f) {}; 

  void setAnimation(const animation_t & a) { 
    _anim = a; 
  }

  const frame_t & getNextFrame(float dt) { 
    _current_time += dt;  

    if(_current_time > _time_limit) { 
      _current_time = 0.0f; 
      if(++_current_frame > (_anim.size()-1)) {
        restart(); 
      }
    }
    
    return _anim[_current_frame]; 
  }

  const frame_t & getFrame(int index) { 
    if( index < _anim.size() ) { 
      return _anim[index]; 
    }
    else { 
      std::cout << "Programmer error\n"; 
    }
  }

  void restart() { 
    _current_frame = 0; 
    _current_time = 0.0f; 
  }

  private: 
    animation_t _anim;         
    int _current_frame; 
    float _current_time; 
    float _time_limit; 
};

class AnimationManager { 
  private: 

    // Variables
    std::vector<Animation> _animations; 
    int _current_anim; 

    const frame_t & getNextFrame(int anim, float dt) { 
      if(_current_anim <= (_animations.size()-1)) {
        return _animations[_current_anim].getNextFrame(dt); 
      }
      else { 
        std::cout << "programmer error\n"; 
      }
    }
 public: 
    AnimationManager() :_current_anim(0) { }
    int pushAnim(const Animation &anim) { 
      _animations.push_back(anim); 
      return (_animations.size()-1); 
    }
    void setAnim(int anim) { 
      if( _current_anim != anim ) { 
        if(_current_anim <= (_animations.size()-1)) {
          _animations[_current_anim].restart(); 
        }
        _current_anim = anim; 
      }
    }
    int getAnim() { 
      return _current_anim; 
    }
    const frame_t & getFrame(int index) { 
      if(_current_anim < _animations.size()) { 
        return _animations[_current_anim].getFrame(index); 
      }
    }
    const frame_t & play(int anim, float dt) { 
      setAnim(anim); 
      return getNextFrame(anim, dt);
    }

};

#endif
