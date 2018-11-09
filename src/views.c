#include "views.h"

typedef struct socketView {
  raw_packet_t **raw;
  GtkListStore *store;
} socketView_t;

static void *fill_list(void *data) {
  socketView_t *view = data;
  raw_packet_t *tmp = NULL;
  GtkTreeIter iter;

  while (1) {
    if ((*view->raw) != NULL) {
      printf("%d\n", (*view->raw)->num);
      if ((tmp == NULL) || (/*(*view->raw)->num != tmp->num)*/1 == 1)) {
        tmp = *view->raw;
        gtk_list_store_append(view->store, &iter);
        gtk_list_store_set(view->store, &iter, COLUMN_NUMBER, tmp->num,
                                          COLUMN_SOURCE, tmp->ip->src_ip,
                                          COLUMN_DEST, tmp->ip->dest_ip,
                                          COLUMN_PULSE, 0,
                                          COLUMN_ICON, "",
                                          COLUMN_ACTIVE, FALSE,
                                          COLUMN_SENSITIVE, FALSE,
                                          -1);
      }
    }
  }
  return NULL;
}

static GtkListStore *create_model(void) {
	return gtk_list_store_new(NUM_COLUMNS, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_BOOLEAN);
}

static void add_columns(GtkTreeView *treeview) {
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Id", renderer, "text", COLUMN_NUMBER, NULL);
	gtk_tree_view_column_set_sort_column_id(column, COLUMN_NUMBER);
	gtk_tree_view_append_column(treeview, column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Source", renderer, "text", COLUMN_SOURCE, NULL);
	gtk_tree_view_column_set_sort_column_id(column, COLUMN_SOURCE);
	gtk_tree_view_append_column(treeview, column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Destination", renderer, "text", COLUMN_DEST, NULL);
	gtk_tree_view_column_set_sort_column_id(column, COLUMN_DEST);
	gtk_tree_view_append_column(treeview, column);
}

static void callback(GtkWidget *widget, gpointer data) {
	(void)widget;
    g_print ("Hello again - %s was pressed\n", (char *)data);
}

static GtkWidget *create_actions_widget(void) {
	GtkWidget *frame = gtk_frame_new("Actions");
	GtkWidget *bbox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
	GtkWidget *button;

	gtk_container_set_border_width(GTK_CONTAINER(bbox), 5);
	gtk_container_add(GTK_CONTAINER(frame), bbox);

	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox), GTK_BUTTONBOX_SPREAD);
	gtk_box_set_spacing(GTK_BOX(bbox), 1);

	button = gtk_button_new_with_label("Start");
    g_signal_connect(button, "clicked", G_CALLBACK(callback), (gpointer) "cool button");
	gtk_container_add(GTK_CONTAINER(bbox), button);

	button = gtk_button_new_with_label("Stop");
	g_signal_connect(button, "clicked", G_CALLBACK(callback), (gpointer) "cool button");
	gtk_container_add(GTK_CONTAINER(bbox), button);

	button = gtk_button_new_with_label("Import");
	g_signal_connect(button, "clicked", G_CALLBACK(callback), (gpointer) "cool button");
	gtk_container_add(GTK_CONTAINER(bbox), button);

	button = gtk_button_new_with_label("Export");
	g_signal_connect(button, "clicked", G_CALLBACK(callback), (gpointer) "cool button");
	gtk_container_add(GTK_CONTAINER(bbox), button);

	return frame;
}

static GtkWidget *create_list_widget(raw_packet_t **raw) {
	GtkWidget *list = gtk_scrolled_window_new(NULL, NULL);
  GtkListStore *store = create_model();
  GtkTreeModel *model = GTK_TREE_MODEL(store);
  GtkWidget *treeview = gtk_tree_view_new_with_model(model);
  pthread_t thread;
  socketView_t *data = malloc(sizeof(*data));
  data->raw = raw;
  data->store = store;

  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(list), GTK_SHADOW_ETCHED_IN);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(list), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
  g_object_unref(model);
  gtk_container_add(GTK_CONTAINER(list), treeview);
  add_columns(GTK_TREE_VIEW(treeview));
  pthread_create(&thread, NULL, fill_list, (void*)data);
	return list;
}

void rawSocketView(GtkWidget *window, raw_packet_t **raw) {
	gtk_window_set_title(GTK_WINDOW(window), "Sockets reader");
	gtk_container_set_border_width(GTK_CONTAINER(window), 8);

	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
	gtk_container_add(GTK_CONTAINER(window), box);

	gtk_box_pack_start(GTK_BOX(box), create_actions_widget(), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), create_list_widget(raw), TRUE, TRUE, 0);

	gtk_widget_show_all(window);
}
