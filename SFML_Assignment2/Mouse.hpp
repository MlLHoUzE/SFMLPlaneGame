
 #ifndef SFML_MOUSE_HPP
 #define SFML_MOUSE_HPP

 // Headers
 #include <SFML/Window/Export.hpp>
 #include <SFML/System/Vector2.hpp>


 namespace sf
 {
	 class Window;
	
		 class SFML_WINDOW_API Mouse
		 {
		 public:
			
			   enum Button
				     {
				         Left,
					         Right,
					         Middle,
					         XButton1,
					         XButton2,
				
					         ButtonCount
					     };
			
				     static bool isButtonPressed(Button button);
			
				     static Vector2i getPosition();
			
				     static Vector2i getPosition(const Window& relativeTo);
			
				     static void setPosition(const Vector2i& position);
			
				     static void setPosition(const Vector2i& position, const Window& relativeTo);
			};
	
		 } // namespace sf


 #endif // SFML_MOUSE_HPP

