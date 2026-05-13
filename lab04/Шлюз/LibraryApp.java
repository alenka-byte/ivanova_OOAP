import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.util.List;

public class LibraryApp extends JFrame {
    private static final String DB_URL = "jdbc:mysql://localhost/lib?serverTimezone=Europe/Moscow&useSSL=false&allowPublicKeyRetrieval=true";
    private static final String DB_USER = "root";
    private static final String DB_PASSWORD = "Cjkysirj1!";

    private LibraryGateway gateway;
    private JTextField searchField;
    private JTextArea resultArea;
    private JButton searchButton;
    private JComboBox<String> sortCombo;

    public LibraryApp() {
        gateway = new MySQLBookGateway(DB_URL, DB_USER, DB_PASSWORD);

        setTitle("Моя библиотека");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(650, 550);
        setLocationRelativeTo(null);
        setLayout(new BorderLayout(10, 10));
        ((JPanel) getContentPane()).setBorder(new EmptyBorder(10, 10, 10, 10));

        // Верхняя панель: поиск + сортировка
        JPanel topPanel = new JPanel(new BorderLayout(5, 5));
        
        // Строка поиска
        JPanel inputPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        inputPanel.add(new JLabel("Поиск:"));
        searchField = new JTextField(25);
        inputPanel.add(searchField);
        searchButton = new JButton("🔍 Найти");
        inputPanel.add(searchButton);
        topPanel.add(inputPanel, BorderLayout.NORTH);
        
        // Панель сортировки
        JPanel sortPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        sortPanel.add(new JLabel("Сортировка:"));
        String[] sortOptions = {"По названию (А-Я)", "По оценке (возрастание)", "По оценке (убывание)"};
        sortCombo = new JComboBox<>(sortOptions);
        sortCombo.addActionListener(e -> performSearch());
        sortPanel.add(sortCombo);
        topPanel.add(sortPanel, BorderLayout.CENTER);
        
        add(topPanel, BorderLayout.NORTH);

        // Область вывода результатов
        resultArea = new JTextArea();
        resultArea.setEditable(false);
        resultArea.setFont(new Font("Monospaced", Font.PLAIN, 13));
        resultArea.setBackground(new Color(250, 250, 250));
        JScrollPane scroll = new JScrollPane(resultArea);
        scroll.setBorder(BorderFactory.createTitledBorder("Результаты поиска"));
        add(scroll, BorderLayout.CENTER);

        // Нижняя панель: добавление книги
        JPanel bottomPanel = new JPanel();
        JButton addButton = new JButton("➕ Добавить новую книгу");
        bottomPanel.add(addButton);
        add(bottomPanel, BorderLayout.SOUTH);

        // Обработчики
        searchButton.addActionListener(e -> performSearch());
        searchField.addActionListener(e -> performSearch());
        addButton.addActionListener(e -> showAddBookDialog());

        setVisible(true);
    }

    // Возвращает строку ORDER BY на основе выбора в комбобоксе
    private String getOrderByClause() {
        switch (sortCombo.getSelectedIndex()) {
            case 0: return "title";
            case 1: return "mark ASC";
            case 2: return "mark DESC";
            default: return "title";
        }
    }

    private void performSearch() {
        String keyword = searchField.getText().trim();
        String orderBy = getOrderByClause();

        resultArea.setText("Загрузка...");
        searchButton.setEnabled(false);

        SwingWorker<List<Book>, Void> worker = new SwingWorker<List<Book>, Void>() {
            @Override
            protected List<Book> doInBackground() {
                if (keyword.isEmpty()) {
                    return gateway.getAllBooks(orderBy);
                } else {
                    return gateway.searchBooks(keyword, orderBy);
                }
            }

            @Override
            protected void done() {
                try {
                    List<Book> books = get();
                    if (books.isEmpty()) {
                        if (keyword.isEmpty())
                            resultArea.setText("Библиотека пуста. Добавьте книги.");
                        else
                            resultArea.setText("Книги не найдены.\nПопробуйте другой запрос или добавьте книгу.");
                    } else {
                        StringBuilder sb = new StringBuilder();
                        if (keyword.isEmpty())
                            sb.append("Все книги (с учётом сортировки):\n\n");
                        else
                            sb.append("Найдено книг: ").append(books.size()).append("\n\n");
                        for (Book b : books) {
                            sb.append(b.toString()).append("\n\n");
                        }
                        resultArea.setText(sb.toString());
                        resultArea.setCaretPosition(0);
                    }
                } catch (Exception ex) {
                    resultArea.setText("Ошибка: " + ex.getMessage());
                    ex.printStackTrace();
                } finally {
                    searchButton.setEnabled(true);
                }
            }
        };
        worker.execute();
    }

    private void showAddBookDialog() {
        JPanel panel = new JPanel(new GridLayout(0, 2, 5, 5));
        panel.add(new JLabel("Название:"));
        JTextField titleField = new JTextField(20);
        panel.add(titleField);
        panel.add(new JLabel("Автор:"));
        JTextField authorField = new JTextField(20);
        panel.add(authorField);
        panel.add(new JLabel("Год издания:"));
        JTextField yearField = new JTextField(20);
        panel.add(yearField);
        panel.add(new JLabel("Оценка (1-10):"));
        JTextField markField = new JTextField(20);
        panel.add(markField);

        Object[] options = {"Добавить", "Отмена"};
        int result = JOptionPane.showOptionDialog(this, panel,
                "Добавление новой книги",
                JOptionPane.OK_CANCEL_OPTION,
                JOptionPane.PLAIN_MESSAGE,
                null,
                options,
                options[0]);

        if (result == JOptionPane.OK_OPTION) {
            String title = titleField.getText().trim();
            String author = authorField.getText().trim();
            String yearStr = yearField.getText().trim();
            String markStr = markField.getText().trim();

            if (title.isEmpty() || author.isEmpty() || yearStr.isEmpty() || markStr.isEmpty()) {
                JOptionPane.showMessageDialog(this, "Заполните все поля", "Ошибка", JOptionPane.ERROR_MESSAGE);
                return;
            }
            int year, mark;
            try {
                year = Integer.parseInt(yearStr);
                mark = Integer.parseInt(markStr);
                if (mark < 1 || mark > 10) throw new NumberFormatException();
            } catch (NumberFormatException e) {
                JOptionPane.showMessageDialog(this, "Год – число, оценка от 1 до 10", "Ошибка", JOptionPane.ERROR_MESSAGE);
                return;
            }

            Book newBook = new Book(title, author, year, mark);
            boolean success = gateway.addBook(newBook);
            if (success) {
                JOptionPane.showMessageDialog(this, "Книга добавлена!", "Успех", JOptionPane.INFORMATION_MESSAGE);
                performSearch();
            } else {
                JOptionPane.showMessageDialog(this, "Ошибка добавления", "Ошибка", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    public static void main(String[] args) {
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
        } catch (ClassNotFoundException e) {
            JOptionPane.showMessageDialog(null, "MySQL JDBC Driver не найден!", "Ошибка", JOptionPane.ERROR_MESSAGE);
            System.exit(1);
        }
        SwingUtilities.invokeLater(LibraryApp::new);
    }
}