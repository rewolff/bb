import java.awt.*;
import java.io.*;
import java.util.List;
import javax.swing.*;

public class bb_gui extends JApplet {
	private static final long serialVersionUID = 1L;
	static JLabel emptyLabel;
    
    public static void main(String[] args) {
    	JApplet gui = new bb_gui();
    	gui.init();
        JFrame frame = new JFrame("BB");
        frame.setContentPane(gui);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        // Display the window.
        frame.pack();
        frame.setVisible(true);
    }
    
    @Override
    public void init() {
        // Create and show this application's GUI.
        try {
			javax.swing.SwingUtilities.invokeAndWait(new Runnable() {
			    public void run() {
			        createAndShowGUI();
			    }
			});
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
    
    private class ProgressInfo {
        private String newValue;
        
        private ProgressInfo (String newValue) {
                this.newValue = newValue;
        }
    }

    private void createAndShowGUI() {
        // Create and set up the window.
 
        emptyLabel = new JLabel("Hoi", null, JLabel.CENTER);
        emptyLabel.setFont(new Font("Courier New", Font.BOLD, 256));
        emptyLabel.setPreferredSize(new Dimension(500, 300));
        getContentPane().add(emptyLabel, BorderLayout.CENTER);
 
        // Launch the monitor thread
        monitorBB bb = new monitorBB();
        bb.execute();
    }

    private class monitorBB extends SwingWorker<Void, ProgressInfo> {
        Process bb;
        
        protected void process(List<ProgressInfo> sets) {
        	processProgress(sets);
        }
      
		@Override
		protected Void doInBackground() {
            try {
				bb = Runtime.getRuntime().exec("/home/knop/bb/bb_mon testbitje");
				InputStream inputStream = bb.getInputStream();
				InputStreamReader ir = new InputStreamReader(inputStream);
				BufferedReader br = new BufferedReader(ir);
				String line;
				while (null != (line = br.readLine())) {
					//System.out.println("Received line from bb_mon\"" + line + "\"");
					publish (new ProgressInfo(line));
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			// TODO Auto-generated method stub
			return null;
		}
    	
    }

    void processProgress(List<ProgressInfo> sets) {
    	for (int i = 0; i < sets.size(); i++) {
    		ProgressInfo currentSet = sets.get(i);
    		//System.out.println("Received line from background process \""+ currentSet.newValue + "\"");
    		emptyLabel.setText(currentSet.newValue);
    	}
    }
}
