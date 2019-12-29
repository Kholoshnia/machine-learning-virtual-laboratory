import java.awt.*;

public class Menu {
    Menu() {
      Frame fr = new Frame();

      Label lb = new Label("Button: "); 
      fr.add(lb);

      Button button = new Button("ButtonText");
      button.setBounds(50, 50, 50, 50);
      fr.add(button);

      fr.setSize(250, 250);
      fr.setLayout(new FlowLayout());
            
      fr.setVisible(true);       
   }

   public static void main(String args[]) {
       Menu ex = new Menu();
   }
}