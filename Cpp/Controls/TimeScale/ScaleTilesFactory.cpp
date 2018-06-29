#include "ScaleTilesFactory.h"

#include <QFont>
#include <QPainter>
#include <QFontMetrics>

namespace
{
const int FONT_SIZE = 12;
const int LABEL_MARGIN = 5;

const double SCALE_HEIGHT = 150;

const double MARKS_DISTANCE = 10;
const double MARK_HEIGHT = 4 * SCALE_HEIGHT / 5;
const double SUBMARK_HEIGHT = 3 * MARK_HEIGHT / 5;
const double SUBMARK_ADD = (MARK_HEIGHT - SUBMARK_HEIGHT) / 4;

const QColor SCALE_LINES_COLOR(Qt::black);
const QColor SCALE_BACKGROUND_COLOR("#d1d1e0");

unsigned GetSpanCount(double width)
{
    static const int max = (std::numeric_limits<int>::max() - 2);

    for (int i = 2; i < max; i += 2)
    {
        const int tmp = (width / i);

        if (tmp <= MARKS_DISTANCE)
            return std::max(1, (i - 2));
    }

    return 1;
}

} // anonymous namespace

ScaleTilesFactory::ScaleTilesFactory(double width, const TimeInterval& interval, time_utils::DurationGroup group)
    : size(width, SCALE_HEIGHT)
    , from(interval.from)
    , count(time_utils::group_count(interval.duration, group))
    , group(group)
{
    if (!interval.duration || !count)
        return;

    QFont font;
    QFontMetrics fm(font);
    font.setPixelSize(FONT_SIZE);

    const bool seconds = (group >= time_utils::DurationGroup::Seconds);
    const QString testStr = time_utils::to_string(0, seconds ? time_utils::DurationGroup::Seconds : group).c_str();

    double tileWidth = (1.2 * fm.size(Qt::TextSingleLine, testStr).width());
    const double totalWidth = (tileWidth * count);

    if (!seconds && (totalWidth <= width))
        tileWidth += ((width - totalWidth) / count);

    size.setWidth(tileWidth);
    step = (interval.duration / count);
    spanCount = GetSpanCount(tileWidth);
}

ScaleTilesFactory ScaleTilesFactory::create(double width, const TimeInterval& interval)
{
    static const int first = (int(time_utils::DurationGroup::Top) + 1);
    static const int last = (int(time_utils::DurationGroup::Last) - 1);

    for (int i = last; i >= first; --i)
    {
        ScaleTilesFactory info(width, interval, time_utils::DurationGroup(i));

        if ((info.size.width() * info.count) <= width)
            return info;
    }

    return ScaleTilesFactory(width, interval, time_utils::DurationGroup::Days);
}

QImage ScaleTilesFactory::tile(int index) const
{
    Q_ASSERT((index >= 0) && (index < count));

    QImage image(size, QImage::Format_ARGB32);
    QPainter painter(&image);

    const double width = size.width();

    painter.setPen(SCALE_LINES_COLOR);
    painter.fillRect(QRect(QPoint(), size), SCALE_BACKGROUND_COLOR);

    // Top
    {
        painter.drawLine(QPointF(), QPointF(width, 0));
    }

    // Pins
    {
        const double spanSize = (width / spanCount);

        for (unsigned i = 0; i < spanCount; ++i)
        {
            const double pos = i * spanSize;
            const double h = i ? SUBMARK_HEIGHT : MARK_HEIGHT;
            const int add = ((i != 0) && !(i % 2)) ? SUBMARK_ADD : 0;

            painter.drawLine(QPointF(pos, 0), QPointF(pos, h + add));
        }
    }

    // Label
    {
        QFont font;
        font.setPixelSize(FONT_SIZE);

        static const QPointF labelPos(LABEL_MARGIN, MARK_HEIGHT);

        const std::time_t value = (from + (step * index));
        const std::string& text = time_utils::to_string(value, group);

        painter.setFont(font);
        painter.drawText(labelPos, QString::fromStdString(text));
    }

    // Bottom
    {
        const double height = (size.height() - 1);
        painter.drawLine(QPointF(0, height), QPointF(width, height));
    }

    return image;
}
