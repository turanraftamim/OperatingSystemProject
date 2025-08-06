package incrementapp;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.*;
import javafx.stage.Stage;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.util.Duration;
import javafx.geometry.Pos;

public class IncrementApp extends Application {
    private volatile long counter = 0;
    private Label counterLabel;
    private TextField intervalField;
    private Timeline uiUpdater;

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        counterLabel = new Label("0");
        counterLabel.setStyle("-fx-font-size: 36px; -fx-font-weight: bold;");

        Label updateText = new Label("Update label after every");
        intervalField = new TextField("1000");
        intervalField.setMaxWidth(100);

        Label msText = new Label("milliseconds");

        Button setIntervalBtn = new Button("Update");
        setIntervalBtn.setOnAction(e -> updateInterval());

        HBox intervalBox = new HBox(5, updateText, intervalField, msText);
        intervalBox.setAlignment(Pos.CENTER);

        VBox root = new VBox(20, counterLabel, intervalBox, setIntervalBtn);
        root.setAlignment(Pos.CENTER);
        root.setStyle("-fx-padding: 30px;");

        Scene scene = new Scene(root, 350, 200);

        primaryStage.setTitle("Multithreading");
        primaryStage.setScene(scene);
        primaryStage.show();

        startCounterThread();
        startUIUpdater(1000);
    }

    private void startCounterThread() {
        Thread counterThread = new Thread(() -> {
            while (true) {
                counter++;
            }
        });
        counterThread.setDaemon(true);
        counterThread.start();
    }

    private void startUIUpdater(int intervalMs) {
        uiUpdater = new Timeline(
            new KeyFrame(Duration.millis(intervalMs), e ->
                counterLabel.setText(String.valueOf(counter))
            )
        );
        uiUpdater.setCycleCount(Timeline.INDEFINITE);
        uiUpdater.play();
    }

    private void updateInterval() {
        try {
            int interval = Integer.parseInt(intervalField.getText());
            uiUpdater.stop();
            startUIUpdater(interval);
        } catch (NumberFormatException ex) {
            intervalField.setText("Invalid number!");
        }
    }
}


