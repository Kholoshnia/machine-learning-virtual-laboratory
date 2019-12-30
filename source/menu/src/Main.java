import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.Text;
import javafx.stage.Stage;

public class Main extends Application {

    @Override
    public void start(Stage stage) throws Exception{

        Text text = new Text(0, 100, "Text");
        text.setFont(Font.font(20));
        text.setFill(Color.GREEN);

        Group root = new Group();
        Scene scene = new Scene(root, 400, 400);

        root.getChildren().add(text);

        stage.setTitle("Hello World");
        stage.setScene(scene);
        stage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}