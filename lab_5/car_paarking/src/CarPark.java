import java.util.concurrent.Semaphore;

public class CarPark {
    private static Integer[] parkingPlaces;
    private static Semaphore parkingController;

    public static void main(String[] args) throws InterruptedException {
        int numPlaces = 10;
        parkingPlaces = new Integer[numPlaces + 1];
        parkingController = new Semaphore(numPlaces, true);
        for (int i = 1; true; i++) {
            new Thread(new Car(i)).start();
            Thread.sleep(500);
        }
    }

    public static class Car implements Runnable {
        private int carId;
        private int parkingPlace = 0;

        Car(int carId) {
            this.carId = carId;
        }

        @Override
        public void run() {
            try {
                System.out.println("Car with id = " + this.carId + " wants to park");
                parkingController.acquire();
                synchronized (parkingPlaces) {
                    for (int i = 1; i < parkingPlaces.length; i++) {
                        if (parkingPlaces[i] == null) {
                            parkingPlaces[i] = this.carId;
                            parkingPlace = i;
                            break;
                        }
                    }
                }
                System.out.println("Car with id = " + this.carId + " parked on place " + this.parkingPlace);

                Thread.sleep(10000);

                synchronized (parkingPlaces) {
                    parkingPlaces[this.parkingPlace] = null;
                }
                parkingController.release();
                System.out.println("Car with id = " + this.carId + " leaved parking");
            } catch (InterruptedException e) {
                System.out.println(e.getMessage());
            }
        }
    }
}
