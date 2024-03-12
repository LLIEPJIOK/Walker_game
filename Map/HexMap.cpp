#include "HexMap.h"
#include "Engine/DataBase.h"
#include "Engine/Turn.h"

void HexMap::end_movement()
{
    emit update_roll();
    Turn* turn = Turn::get_Turn();

    if (turn->get_already_moved())
    {
        if (Transceiver::get_transceiver()->get_id() == turn->get_player()->get_id() - 1)
            emit event_triggered(); // запуск цепочки обработок всех действий, происходящих после конца движения
    }
}

HexMap::HexMap(QWidget *parent, GraphMap* map_) : QGraphicsView(parent) {
    map = map_;
    setMouseTracking(true);
    mousePosition = QPoint(-1, -1);
    start = { 0, 0 };

    offset = QPoint(0, 0);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    screen_size = QApplication::screens().at(0)->size();
    battle_map_size.first = DataBase::get_DataBase()->get_graph_map()->getWidth();
    battle_map_size.second = DataBase::get_DataBase()->get_graph_map()->getHeight();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &HexMap::initialize);
    graphics_scene = new QGraphicsScene(this);

    setRenderHint(QPainter::Antialiasing, true);
    continue_moving = false;

    int w = map->getWidth() * 3 * HexMathOnScreen::size / 2 - HexMathOnScreen::size / 2;
    int h =  map->getHeight() * std::sqrt(3) * HexMathOnScreen::size;
    source_pixmap = QPixmap(w, h);


    source_pixmap_painter.begin(&source_pixmap);
    source_pixmap_painter.setBrush(QBrush(QImage(":/tiles/Pictures/Tiles/forest_8.png")));

    timer->start(50);

    connect(Transceiver::get_transceiver(), &Transceiver::move_to, this, &HexMap::move_to);
}

QPoint HexMap::get_offset() const noexcept
{
    return offset;
}

void HexMap::set_x_offset(const int& x_offset_)
{
    offset.setX(x_offset_);
}

void HexMap::set_y_offset(const int &y_offset_)
{
    offset.setY(y_offset_);
}

QSize HexMap::get_source_pixmap_size() const
{
    return source_pixmap.size();
}

void HexMap::set_current_players_model(int index)
{
    current_players_model = players_on_map[index];
}

void HexMap::next_players_model()
{
    auto turn_number = Turn::get_Turn()->get_turn_number();
    current_players_model = players_on_map[(turn_number - 1) % players_on_map.size()];
}

void HexMap::update_current_area(QPoint position)
{
    position.setY(battle_map_size.second * position.y() / battle_map_size.first);

    QPixmap pixmap(this->width(), this->height());
    QPainter painter(&pixmap);
    source_pixmap_painter.setBrush(QBrush(QImage(":/tiles/Pictures/Tiles/forest_8.png")));

    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawPixmap(QRect(0, 0, this->width(), this->height()), source_pixmap, QRect(position, QSize(this->width(), this->height())));


    pixmap_item->setPixmap(pixmap);
    pixmap_item->setPos(position);
    setSceneRect(QRect(position, QSize(this->width(), this->height())));
    offset = position;
}

void HexMap::move_to_player()
{
    auto players_pos = current_players_model->pos();
    players_pos.setX(players_pos.x() - this->width() / 2);
    players_pos.setY(players_pos.y() - this->height() / 2);

    if(players_pos.x() < 0)
        players_pos.setX(0);
    if(players_pos.y() < 0)
        players_pos.setY(0);
    if(players_pos.x() + this->width() > source_pixmap.width())
        players_pos.setX(source_pixmap.width() - this->width());
    if(players_pos.y() + this->height() > source_pixmap.height())
        players_pos.setY(source_pixmap.height() - this->height());

    update_current_area(players_pos.toPoint());
    emit area_was_changed(players_pos.toPoint());
}

void HexMap::want_to_move()
{
    source_pixmap_painter.setBrush(QBrush(QImage(":/tiles/Pictures/Tiles/forest_8.png")));
    auto player_pos = current_players_model->pos();
    int roll = Turn::get_Turn()->get_roll();

    ways = Coordinates::range(HexMathOnScreen::pixelToHex(player_pos.toPoint()), roll, map);

    trace_hexes(ways, QColor(255, 0, 0));

    QPixmap pixmap(this->width(), this->height());
    QPainter painter(&pixmap);
    painter.drawPixmap(QRect(0, 0, this->width(), this->height()), source_pixmap, QRect(offset, QSize(this->width(), this->height())));
    pixmap_item->setPixmap(pixmap);

}

void HexMap::trace_hexes(std::unordered_set<Location> hexes, QColor trace_color)
{
    source_pixmap_painter.setPen(trace_color);

    for(const auto& way : ways) {
        draw_hex(way);
    }
}

void HexMap::draw_hex(const Location& location)
{
    QPolygonF polygon;
    auto hex_corners = HexMathOnScreen::corners(HexMathOnScreen::hexToPixel(location));
    for(const auto& i : hex_corners) {
        polygon << i;
    }

    source_pixmap_painter.drawPolygon(polygon);
}

void HexMap::update_player_coords(Player *player)
{
    auto i = player->get_id() - 1;
    players_on_map[i]->setPos(HexMathOnScreen::hexToPixel(player->get_players_position()));
}

void HexMap::update_coords()
{
    for (const auto& i : players_on_map)
    {
        if (i == nullptr)
            continue;

        update_player_coords(i->get_connected_player());
    }
}

std::vector<PlayersModel *>* HexMap::get_players()
{
    return &players_on_map;
}

void HexMap::mousePressEvent(QMouseEvent *event)
{
    if(!ways.empty() && event->button() == Qt::LeftButton) {
        Location location = HexMathOnScreen::pixelToHex(event->pos() + offset);
        if(std::find(ways.begin(), ways.end(), location) != ways.end())
            prepare_for_move(location);
    }
}

void HexMap::initialize()
{
    timer->stop();
    disconnect(timer, &QTimer::timeout, this, &HexMap::initialize);
    connect(timer, &QTimer::timeout, this, &HexMap::player_move);

    graphics_scene->setSceneRect(0, 0, this->width(), this->height());
    graphics_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(graphics_scene);

    auto sequence = DataBase::get_DataBase()->get_sequence();

    source_pixmap_painter.setRenderHint(QPainter::Antialiasing);
    source_pixmap_painter.setBrush(QBrush(QImage(":/tiles/Pictures/Tiles/forest_8.png")));

    for(const auto& i : map->getMap()) {
        draw_hex(i.first);
    }

    pixmap_item = graphics_scene->addPixmap(source_pixmap);
    pixmap_item->setPos(0, 0);

    for(int i = 0; i < sequence->size(); i++)
    {
        PlayersModel *players_model = new PlayersModel(graphics_scene, 1 * HexMathOnScreen::size, 1 * HexMathOnScreen::size, icons[i]);

        connect(players_model, &PlayersModel::target_to_attack, this, &HexMap::process_attack);

        players_on_map.push_back(players_model);
        players_model->set_connected_player(sequence->at(i));
        graphics_scene->addItem(players_model);
    }

    update_coords();
    current_players_model = players_on_map[Turn::get_Turn()->get_player()->get_id() - 1];
    emit was_initialized();
}

void HexMap::player_move()
{
    auto turn = Turn::get_Turn();

    QPointF to_move(0, 0);
    auto player_pos = current_players_model->pos().toPoint();

    QPoint destination = QPoint(coordinate_path.first().toPoint().x() - 50, coordinate_path.first().toPoint().y() - 50);



    if(player_pos.x() > destination.x())
        to_move.setX(-1);
    if(player_pos.x() < destination.x())
        to_move.setX(1);
    if(player_pos.y() > destination.y())
        to_move.setY(-1);
    if(player_pos.y() < destination.y())
        to_move.setY(1);

    current_players_model->moveBy(to_move.x(), to_move.y());
    move_to_player();

    game_msg msg = {turn->get_player()->get_id() - 1, turn->get_player()->get_id() - 1, 8, turn->get_roll(), {char(to_move.x()), char(to_move.y())}};
    Transceiver::get_transceiver()->send_msg(msg);

    if(player_pos == destination) {
        current_players_model->get_connected_player()->set_players_position(HexMathOnScreen::pixelToHex(coordinate_path.dequeue().toPoint()));
        turn->set_roll(turn->get_roll() - 1);
    }

    if(coordinate_path.isEmpty()) {
        timer->stop();
        if(turn->get_roll()){
            want_to_move();
        }
        else {
            Turn::get_Turn()->process_end_of_movement();
            end_movement();
        }
    }
}

void HexMap::process_attack(PlayersModel* attacked_player)
{

    Turn* turn = Turn::get_Turn();
    if (Transceiver::get_transceiver()->get_id() != turn->get_player()->get_id() - 1)
        return;

    auto current_player_coordinate_pos = current_players_model->pos();
    auto attack_range = Coordinates::range(HexMathOnScreen::pixelToHex(current_player_coordinate_pos.toPoint()), turn->get_player()->get_characteristics()["RNG"], map);
    auto attacked_player_pos = HexMathOnScreen::pixelToHex(attacked_player->pos().toPoint());
    QString attacked_name = QString::fromStdString(attacked_player->get_connected_player()->get_name());

    if(turn->get_moving())
    {
        emit action(tr("You cannot attack while moving!"));
        return;
    }
    if(turn->get_has_attacked() /*&& s.find(player)!=s.end()*/)
        emit action(tr("You have already attacked during this turn"));
    else
    {
        if(attack_range.find(attacked_player_pos) != attack_range.end())
        {
            auto atk = turn->get_player()->attack(attacked_player->get_connected_player());
            if(turn->get_player()->get_killed_player() != -1)
            {
                game_msg lose_msg = {Transceiver::get_transceiver()->get_id(), turn->get_player()->get_killed_player(), 666, 0};
                Transceiver::get_transceiver()->send_msg(lose_msg);

                emit action(tr("You killed") + " " + attacked_name + " " + tr("after dealing") + " " + QString::number(atk.first) + " " + tr("damage") + " (" + tr("critical strike") + ")");

                process_killed_player(turn->get_player()->get_killed_player());
                turn->set_has_attacked(true);
                return;
            }
            else if (atk.second)
                emit action(tr("You attacked") + " " + QString::fromStdString(attacked_player->get_connected_player()->get_name()) + " " + tr("dealing") + " " + QString::number(atk.first) + " " + tr("damage") + " (" + tr("critical strike") + ")");
            else
                emit action(tr("You attacked") + " " + QString::fromStdString(attacked_player->get_connected_player()->get_name()) + " " + tr("dealing") + " " + QString::number(atk.first) + " " + tr("damage"));
            turn->set_has_attacked(true);

        }
        else
            emit action(tr("Too far away! Come closer!"));
    }
}

void HexMap::process_killed_player(int place)
{
    delete players_on_map[place];
    players_on_map[place] = nullptr;
    DataBase::get_DataBase()->set_players_alive(DataBase::get_DataBase()->get_players_alive() - 1);
    DataBase::get_DataBase()->set_dead(place);
    if(DataBase::get_DataBase()->get_players_alive() == 1)
        emit win_by_killing();
}

void HexMap::prepare_for_move(const Coordinates::Hex<int> &hex)
{
    trace_hexes(ways, QColor(0, 0, 0));
    ways.clear();

    auto player_pos = current_players_model->pos();
    Location start = HexMathOnScreen::pixelToHex(player_pos.toPoint());

    map->aStarSearch(start, hex);
    auto reconstructed_path = map->reconstruct_path(start, hex);

    for(const auto& i : reconstructed_path) {
        coordinate_path.push_back(HexMathOnScreen::hexToPixel(i));
    }

    Turn::get_Turn()->set_is_moving(true);

    timer->start(10);
}

void HexMap::move_to(game_msg msg)
{
    current_players_model->moveBy(msg.buffer[0], msg.buffer[1]);
    move_to_player();
}
